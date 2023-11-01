#include "discrete_cosine_transform.hpp"

#include <cmath>
#include <memory>
#include <opencv2/core/mat.hpp>
#include <opencv2/opencv.hpp>

// Forward DCT
cv::Mat dct(cv::Mat& block, std::shared_ptr<cv::Mat>& mat) {
    int N = block.rows;
    cv::Mat trans(cv::Size(N, N), block.type());

    double c_i, c_j;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0.0;
            for (int x = 0; x < N; x++) {
                for (int y = 0; y < N; y++) {
                    double cos_i = cos((2 * x + 1) * i * M_PI / (2 * N));
                    double cos_j = cos((2 * y + 1) * j * M_PI / (2 * N));
                    sum += block.at<float>(x, y) * cos_i * cos_j;
                }
            }
            c_i = (i == 0) ? sqrt(1.0 / N) : sqrt(2.0 / N);
            c_j = (j == 0) ? sqrt(1.0 / N) : sqrt(2.0 / N);
            trans.at<float>(i, j) = c_i * c_j * sum;
        }
    }
    return trans;
}

// Inverse DCT
cv::Mat idct(cv::Mat& trans, std::shared_ptr<cv::Mat>& mat) {
    int N = trans.rows;
    cv::Mat block(cv::Size(N, N), trans.type());

    double c_i, c_j;
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            double sum = 0.0;
            for (int i = 0; i < N; i++) {
                for (int j = 0; j < N; j++) {
                    double cos_i = cos((2 * x + 1) * i * M_PI / (2 * N));
                    double cos_j = cos((2 * y + 1) * j * M_PI / (2 * N));
                    c_i = (i == 0) ? sqrt(1.0 / N) : sqrt(2.0 / N);
                    c_j = (j == 0) ? sqrt(1.0 / N) : sqrt(2.0 / N);
                    sum += c_i * c_j * trans.at<float>(i, j) * cos_i * cos_j;
                }
            }
            block.at<float>(x, y) = sum;
        }
    }
    return block;
}

const int NUM_THREADS = std::thread::hardware_concurrency();

void DiscreteCosineTransform::Encode(const boost::filesystem::path &payload_path)
{
    // Ensure that the carrier has enough room for the payload
    if (boost::filesystem::file_size(payload_path) * 8 > this->image_capacity)
    {
        throw EncodeException("Error: Failed to encode payload, carrier too small");
    }

    // Convert the filename to a vector<unsigned char>
    std::string payload_filename = payload_path.filename().string();
    std::vector<unsigned char> filename_bytes(payload_filename.begin(), payload_filename.end());

    // Encode the filename into the carrier image
    this->EncodeChunkLength(0, filename_bytes.size());
    this->EncodeChunk(32, filename_bytes.begin(), filename_bytes.end());

    // Read the payload into a vector<unsigned char>
    std::vector<unsigned char> payload_bytes = this->ReadPayload(payload_path);

    this->EncodeChunkLength(32 + filename_bytes.size() * 8, payload_bytes.size());

    // Determine how many threads to use so that each thread encodes more than 12B
    int encode_threads = NUM_THREADS;

    while ((encode_threads > 1) && ((payload_bytes.size() / encode_threads)) < 12)
    {
        encode_threads--;
    }

    // Encode the payload into the carrier image
    if (encode_threads == 1)
    {
        this->EncodeChunk(64 + (filename_bytes.size() * 8), payload_bytes.begin(), payload_bytes.end());
    }
    else
    {
        std::vector<std::thread> threads;
        threads.reserve(encode_threads);

        for (int i = 0; i < encode_threads; i++)
        {
            threads.push_back(
                    std::thread(&DiscreteCosineTransform::EncodeChunk,
                        this,
                        64 + (filename_bytes.size() * 8) + (((payload_bytes.size() / encode_threads) * 8) * i),
                        payload_bytes.begin() + ((payload_bytes.size() / encode_threads) * i),
                        payload_bytes.end() - ((payload_bytes.size() / encode_threads) * ((encode_threads - 1) - i))));
        }

        // Wait for all the threads to finish encoding
        for (std::thread &thr : threads)
        {
            thr.join();
        }
    }

    // Merge the image channels and convert back to unsigned char
    cv::merge(this->channels, this->image);
    this->image.convertTo(this->image, CV_8U);

    // Write the steganographic image
    cv::imwrite("steg-" + this->image_path.filename().replace_extension(".jpg").string(), this->image,
            std::vector<int>{cv::IMWRITE_JPEG_QUALITY, 100});
}

void DiscreteCosineTransform::Decode()
{
    // Decode the filename from the steganographic image
    unsigned int filename_length = this->DecodeChunkLength(0);
    std::vector<unsigned char> filename_bytes(filename_length);
    this->DecodeChunk(32, filename_bytes.begin(), filename_bytes.end());

    // Convert the filename vector<unsigned char> to a string
    std::string payload_filename(filename_bytes.begin(), filename_bytes.end());

    // Decode the payload length from the steganographic image
    unsigned int payload_length = this->DecodeChunkLength(32 + (filename_length * 8));

    // Determine how many threads to use so that each thread decodes more than 12B
    int decode_threads = NUM_THREADS;

    while ((decode_threads > 1) && (payload_length / decode_threads) < 12)
    {
        decode_threads--;
    }

    // Decode the payload from the steganographic image
    std::vector<unsigned char> payload_bytes(payload_length);

    if (decode_threads == 1)
    {
        this->DecodeChunk(64 + (filename_length * 8), payload_bytes.begin(), payload_bytes.end());
    }
    else
    {
        std::vector<std::thread> threads;
        threads.reserve(decode_threads);

        for (int i = 0; i < decode_threads; i++)
        {
            threads.push_back(
                std::thread(&DiscreteCosineTransform::DecodeChunk,
                    this,
                    64 + (filename_length * 8) + (((payload_length / decode_threads) * 8) * i),
                    payload_bytes.begin() + ((payload_length / decode_threads) * i),
                    payload_bytes.end() - ((payload_length / decode_threads) * ((decode_threads - 1) - i))));
        }

        // Wait for all the threads to finish encoding
        for (std::thread &thr : threads)
        {
            thr.join();
        }
    }

    // Write the decoded payload
    this->WritePayload("steg-" + payload_filename, payload_bytes);
}

void DiscreteCosineTransform::EncodeChunk(const int &start, std::vector<unsigned char>::iterator it, std::vector<unsigned char>::iterator en)
{
    int bit = 0;
    bool loops_initialised = false;

    for (int row = 0; row < this->image.rows - 8; row += 8)
    {
        for (int col = 0; col < this->image.cols - 8; col += 8)
        {
            if (!loops_initialised)
            {
                row = start / ((this->image.cols - 8) / 8) * 8;
                col = start % ((this->image.cols - 8) / 8) * 8;
                loops_initialised = true;
            }

            // The current 8x8 block we are working on
            cv::Mat block(channels[0], cv::Rect(col, row, 8, 8));

            // A "working" copy of the block
            cv::Mat trans(cv::Size(8, 8), block.type());

            // Perform the forward dct
            auto trans1 = std::make_shared<cv::Mat>();
            dct(block, trans);

            // Embed the current chunk bit in the carrier
            this->SwapCoefficients(&trans, this->GetBit(*it, bit % 8));
            auto b = cv::Mat();
            // dct(b, trans);

            // Perform the inverse dct
            idct(trans, block);

            // We have finished embedding, clean up
            if (++bit % 8 == 0 && ++it == en)
            {
                return;
            }
        }
    }
}

void DiscreteCosineTransform::EncodeChunkLength(const int &start, const unsigned int &chunk_length)
{
    int bit = 0;
    bool loops_initialised = false;

    for (int row = 0; row < this->image.rows - 8; row += 8)
    {
        for (int col = 0; col < this->image.cols - 8; col += 8)
        {
            if (!loops_initialised)
            {
                row = start / ((this->image.cols - 8) / 8) * 8;
                col = start % ((this->image.cols - 8) / 8) * 8;
                loops_initialised = true;
            }

            // The current 8x8 block we are working on
            cv::Mat block(channels[0], cv::Rect(col, row, 8, 8));

            // A "working" copy of the block
            cv::Mat trans(cv::Size(8, 8), block.type());

            // Perform the forward dct
            dct(block, trans);

            // Swap N DCT coefficients
            this->SwapCoefficients(&trans, this->GetBit(chunk_length, bit));

            // Perform the inverse dct
            idct(trans, block);

            // We have finished embedding, clean up
            if (++bit == 32)
            {
                return;
            }
        }
    }
}

void DiscreteCosineTransform::DecodeChunk(const int start, std::vector<unsigned char>::iterator it, std::vector<unsigned char>::iterator en)
{
    int bit = 0;
    bool loops_initialised = false;

    for (int row = 0; row < this->image.rows - 8; row += 8)
    {
        for (int col = 0; col < this->image.cols - 8; col += 8)
        {
            if (!loops_initialised)
            {
                row = start / ((this->image.cols - 8) / 8) * 8;
                col = start % ((this->image.cols - 8) / 8) * 8;
                loops_initialised = true;
            }

            // The current 8x8 block we are working on
            cv::Mat block(channels[0], cv::Rect(col, row, 8, 8));

            // A "working" copy of the block
            cv::Mat trans(cv::Size(8, 8), block.type());

            // Perform the forward dct
            dct(block, trans);

            // Read from N swapped DCT coefficients
            this->SetBit(&(*it), bit % 8, (trans.at<float>(0, 2) < trans.at<float>(2, 0)));

            if (++bit % 8 == 0 && ++it == en)
            {
                return;
            }
        }
    }

    throw DecodeException("Error: Failed to decode payload");
}

unsigned int DiscreteCosineTransform::DecodeChunkLength(const int &start)
{
    unsigned int chunk_length = 0;

    int bit = 0;
    bool loops_initialised = false;

    for (int row = 0; row < this->image.rows - 8; row += 8)
    {
        for (int col = 0; col < this->image.cols - 8; col += 8)
        {
            if (!loops_initialised)
            {
                row = start / ((this->image.cols - 8) / 8) * 8;
                col = start % ((this->image.cols - 8) / 8) * 8;
                loops_initialised = true;
            }

            // The current 8x8 block we are working on
            cv::Mat block(channels[0], cv::Rect(col, row, 8, 8));

            // A "working" copy of the block
            cv::Mat trans(cv::Size(8, 8), block.type());

            // Perform the forward dct
            dct(block, trans);

            // Read from N swapped DCT coefficients
            this->SetBit(&chunk_length, bit, (trans.at<float>(0, 2) < trans.at<float>(2, 0)));

            if (++bit == 32)
            {
                // We have decoded the integer, check if it's valid
                if (chunk_length >= this->image_capacity || chunk_length == 0)
                {
                    throw DecodeException("Error: Failed to decode payload length");
                }

                return chunk_length;
            }
        }
    }

    throw DecodeException("Error: Failed to decode payload length");
}

void DiscreteCosineTransform::SwapCoefficients(cv::Mat *block, const int &value)
{
    // Read two coefficients from the image block
    float low = block->at<float>(0, 2);
    float high = block->at<float>(2, 0);

    // Swap the coefficients so that low is low and high is high
    if (value && (low > high))
    {
        std::swap(low, high);
    }
    else if (!value && (low < high))
    {
        std::swap(low, high);
    }

    // Apply the persistence value
    if (value && (low == high || low < high))
    {
        low -= this->persistence;
        high += this->persistence;
    }
    else if (!value && (low == high || low > high))
    {
        low += this->persistence;
        high -= this->persistence;
    }

    // Write the coefficients back to the block
    block->at<float>(0, 2) = low;
    block->at<float>(2, 0) = high;
}

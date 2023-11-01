#include <iostream>
#include <string>
#include <thread>
#include <tuple>
#include <vector>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "steganography.hpp"
#include "exceptions.hpp"

#ifndef DISCRETE_COSINE_TRANSFORM_HPP
#define DISCRETE_COSINE_TRANSFORM_HPP

class DiscreteCosineTransform : public Steganography
{
    public:
        explicit DiscreteCosineTransform(const boost::filesystem::path &image_path, int persistence) : Steganography(image_path)
        {
            this->persistence = persistence;
            this->image_capacity = ((this->image.rows - 8) / 8) * ((this->image.cols - 8) / 8);

            // Convert the image to floating point and split the channels
            this->image.convertTo(this->image, CV_32F);
            cv::split(this->image, this->channels);
        }

        void Encode(const boost::filesystem::path &payload_path);

        void Decode();

    private:
        std::vector<cv::Mat> channels;

        int persistence;

        int image_capacity;

        void EncodeChunk(const int &start, std::vector<unsigned char>::iterator it, std::vector<unsigned char>::iterator en);

        void EncodeChunkLength(const int &start, const unsigned int &chunk_length);

        void DecodeChunk(const int start, std::vector<unsigned char>::iterator it, std::vector<unsigned char>::iterator en);

        unsigned int DecodeChunkLength(const int &start);

        void SwapCoefficients(cv::Mat *block, const int &value);
};

#endif // DISCRETE_COSINE_TRANSFORM_HPP

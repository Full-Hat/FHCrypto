#include <fstream>
#include <iostream>
#include <vector>
#include <boost/filesystem.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "exceptions.hpp"

#ifndef STEGANOGRAPHY_HPP
#define STEGANOGRAPHY_HPP

class Steganography
{
    public:
        explicit Steganography(const boost::filesystem::path &image_path)
        {
            this->image_path = image_path;
            auto str = image_path.string();
            std::cout << str << std::endl;
            this->image = cv::imread(image_path.string(), cv::IMREAD_UNCHANGED);

            if (!this->image.data)
            {
                std::cout << "Try to load " << image_path.c_str() << std::endl;
                throw ImageException("Error: Failed to open input image");
            }
        }

        virtual void Encode(const boost::filesystem::path & payload_path) = 0;

        virtual void Decode() = 0;

    protected:
        boost::filesystem::path image_path;

        cv::Mat image;

        std::vector<unsigned char> ReadPayload(const boost::filesystem::path & payload_path);

        void WritePayload(const boost::filesystem::path &payload_path, const std::vector<unsigned char> &payload);

        template <class T>
        inline void SetBit(T *target, const int &bit, const int &value)
        {
            *target ^= (-(unsigned int)value ^ *target) & (1UL << bit);
        }

        template <class T>
        inline int GetBit(const T &target, const int &bit)
        {
            return (target >> bit) & 1UL;
        }
};

#endif // STEGANOGRAPHY_HPP

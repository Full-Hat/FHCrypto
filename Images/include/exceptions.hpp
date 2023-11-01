#include <stdexcept>

#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

class ImageException : public std::runtime_error
{
    public:
        explicit ImageException(const std::string &message) : std::runtime_error(message) {};
};

class EncodeException : public std::runtime_error
{
    public:
        explicit EncodeException(const std::string &message) : std::runtime_error(message) {};
};

class DecodeException : public std::runtime_error
{
    public:
        explicit DecodeException(const std::string &message) : std::runtime_error(message) {};
};

#endif // EXCEPTIONS_HPP

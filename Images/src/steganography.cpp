#include "steganography.hpp"

std::vector<unsigned char> Steganography::ReadPayload(const boost::filesystem::path &payload_path)
{
    std::ifstream file(payload_path.c_str(), std::ios::binary);
    file.unsetf(std::ios::skipws); // do not skip whitespace

    std::vector<unsigned char> payload;
    payload.reserve(boost::filesystem::file_size(payload_path));
    payload.insert(payload.begin(), std::istream_iterator<unsigned char>(file), std::istream_iterator<unsigned char>());

    file.close();
    return payload;
}

void Steganography::WritePayload(const boost::filesystem::path &payload_path, const std::vector<unsigned char> &payload)
{
    std::ofstream file(payload_path.c_str(), std::ios::binary);

    std::copy(payload.cbegin(), payload.cend(), std::ostream_iterator<unsigned char>(file));

    file.close();
}

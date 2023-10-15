#pragma once

#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <ios>
#include <iterator>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>

class FileReader
{
public:
    using BinaryData_64 = std::pair<std::vector<uint64_t>, size_t>;
    using BinaryData_uint = std::pair<std::vector<unsigned int>, size_t>;
    using BinaryData_8 = std::vector<uint8_t>;
    FileReader(std::string_view _fileName)
    {
        m_file_object = std::ifstream(_fileName.data(), std::ios::binary);
        m_file_size = std::filesystem::file_size(_fileName);

        InitReader(std::numeric_limits<size_t>::max());
    }

    ~FileReader()
    {
        m_file_object.close();
    }

    bool IsOpen() const
    {
        return m_file_object.is_open();
    }

    void InitReader(size_t _max_RAM_Usage)
    {
        // Min size of required RAM is 8 bytes 'cause class uses 64 bits variable
        assert(_max_RAM_Usage >= 8);

        m_block_size = std::min(m_file_size, _max_RAM_Usage);
        
        if (m_block_size % 8 != 0)
        {
            m_block_size = (m_block_size + 7) / 8 * 8;
        }
    }

    BinaryData_64 ReadBlock_64()
    {
        const auto block_8 = ReadBlock<uint64_t>();

        return { block_8.first, block_8.second };
    }

    BinaryData_uint ReadBlock_uint()
    {
        const auto block_8 = ReadBlock<unsigned int>();

        return { block_8.first, block_8.second };
    }

    BinaryData_8 ReadBlock_8()
    {
        const auto block_8 = ReadBlock<uint8_t>();
        return block_8.first;
    }

    static unsigned short GetTrashBytesCount(unsigned short _realSizeOfBlock)
    {
        return 8 - _realSizeOfBlock;
    }

    bool Is_EOF() const
    {
        return m_bytes_returned == m_file_size;
    }

protected:
    /**
     * @brief Read text from file by blocks
     * @details Blocks are generated based on max ram usage and min block size (8 bytes in current implementation). Each call of that function 
     * could return data from file and the real size of last block
     * 
     * @return BinaryData_64 first -- data from file, second -- size of the last block
     */
     template<typename Element>
    std::pair<std::vector<Element>, size_t> ReadBlock()
    {
        if (m_bytes_returned == m_file_size)
        {
            return { {}, 0 };
        }

        // Required bytes to read from file in that interation
        size_t data_size_to_read;

        // If block is larger than remaining text
        if (m_block_size > m_file_size - m_bytes_returned)
        {
            data_size_to_read = m_file_size - m_bytes_returned;
        }
        // If function should read full block
        else
        {
            // Its required 'cause function read text to uint64_t from text with chars
            data_size_to_read = m_block_size;
        }

        size_t data_buffer_size = (data_size_to_read + sizeof(Element) - 1) / sizeof(Element);
        std::vector<Element> data(data_buffer_size);
        
        m_file_object.read(reinterpret_cast<char*>(data.data()), data_size_to_read);

        m_bytes_returned += m_file_object.gcount();

        return { data, m_file_object.gcount() };
    }

protected:
    std::ifstream m_file_object;
    size_t m_block_size;
    size_t m_file_size;
    size_t m_bytes_returned = 0;
};

inline void WriteToFile(const std::vector<uint64_t>& data, const std::string_view filename, size_t real_data_size) {
    std::ofstream file(filename.data(), std::ios::binary);
    if (file.is_open()) {
        if (real_data_size % 8 == 0)
        {
            file.write(reinterpret_cast<const char*>(data.data()), data.size() * 8);
            return;
        }

        file.write(reinterpret_cast<const char*>(data.data()), (data.size() - 1) * 8);

        auto last_block_size = real_data_size - (data.size() - 1) * 8;
        if (last_block_size == 8)
        {
            return;
        }
        for (int i = 0; i < last_block_size; ++i)
        {
            char el = (uint8_t)(data[data.size() - 1] >> (i * 8));
            file.write(&el, 1);
        }
    }
}

inline void WriteToFile(const std::vector<unsigned int>& data, const std::string_view filename, size_t real_data_size) {
    std::ofstream file(filename.data(), std::ios::binary);
    if (file.is_open()) {
        if (real_data_size % 4 == 0)
        {
            file.write(reinterpret_cast<const char*>(data.data()), data.size() * 4);
            return;
        }

        file.write(reinterpret_cast<const char*>(data.data()), (data.size() - 1) * 4);

        auto last_block_size = real_data_size - (data.size() - 1) * 4;
        if (last_block_size == 4)
        {
            return;
        }
        for (int i = 0; i < last_block_size; ++i)
        {
            char el = (uint8_t)(data[data.size() - 1] >> (i * 8));
            file.write(&el, 1);
        }
    }
}

inline void WriteToFile(const std::vector<uint8_t>& data, std::string_view filename) 
{
    // Create an output file stream object with the given filename
    std::ofstream ofs(filename.data(), std::ios::binary);
    // Check if the file stream is open and ready for writing
    if (ofs) {
        // Write the data from the vector to the file stream
        ofs.write(reinterpret_cast<const char*>(data.data()), data.size());
        // Close the file stream
        ofs.close();
    }
    else {
        // Handle the error if the file stream could not be opened
        std::cerr << "Could not open file " << filename << " for writing\n";
    }
}

template <typename InputIterator1, typename InputIterator2>
bool RangeEqual(InputIterator1 begin1, InputIterator1 end1,
                InputIterator2 begin2, InputIterator2 end2)
{
    while (begin1 != end1 && begin2 != end2) {
        if (*begin1 != *begin2) return false;
        ++begin1;
        ++begin2;
    }
    return (begin1 == end1) && (begin2 == end2);
}

inline bool IsEqual(std::string_view _file_name1, std::string_view _file_name2)
{
    std::ifstream file1(_file_name1.data());
    std::ifstream file2(_file_name2.data());

    std::istreambuf_iterator<char> begin1(file1);
    std::istreambuf_iterator<char> begin2(file2);

    std::istreambuf_iterator<char> end;

    return RangeEqual(begin1, end, begin2, end);
}

inline void test_reader()
{
    const std::string input_files[] {
        "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/text.txt",
        "/Users/full-hat/Documents/MyProjects/FHCrypto/Texts/alice29.txt",
        "/Users/full-hat/Documents/MyProjects/FHCrypto/Texts/asyoulik.txt",
    };

    const std::string output_file {
        "/Users/full-hat/Documents/MyProjects/FHCrypto/Texts/a.txt"
    };

    for (auto input_file : input_files)
    {
        // Test ReadBlock_8
        auto file = FileReader(input_file);
        file.InitReader(std::numeric_limits<size_t>::max());

        auto r = file.ReadBlock_8();
        WriteToFile(r, output_file);

        assert(IsEqual(input_file, output_file));
    }

    for (auto input_file : input_files)
    {
        // Test ReadBlock_64
        auto file = FileReader(input_file);
        file.InitReader(std::numeric_limits<size_t>::max());

        auto r = file.ReadBlock_64();
        WriteToFile(r.first, output_file, r.second);

        assert(IsEqual(input_file, output_file));
    }
}

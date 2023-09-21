#pragma once

#include <_types/_uint64_t.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <filesystem>
#include <ios>
#include <limits>
#include <memory>
#include <stdexcept>
#include <string>
#include <string_view>
#include <sys/_types/_int64_t.h>
#include <sys/types.h>
#include <utility>
#include <vector>
#include <fstream>
#include <iostream>

class FileReader
{
public:
    using BinaryData_64 = std::pair<std::vector<uint64_t>, ushort>;
    FileReader(std::string_view _fileName)
    {
        m_file_object = std::ifstream(_fileName, std::ios::binary);
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

    /**
     * @brief Read text from file by blocks
     * @details Blocks are generated based on max ram usage and min block size (8 bytes in current implementation). Each call of that function 
     * could return data from file and the real size of last block
     * 
     * @return BinaryData_64 first -- data from file, second -- size of the last block
     */
    BinaryData_64 ReadBlock()
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

        std::vector<uint64_t> data((data_size_to_read + 7) / 8);
        
        m_file_object.read(reinterpret_cast<char*>(data.data()), data_size_to_read);

        // Calculate if function fills 8 bytes value fully
        size_t last_block_size;
        // if last block size == 0 -> m_file_object.gcount() & 7 + 8
        // else                       m_file_object.gcount() & 7
        // & 7 -- just a fatser way to calculate % 8
        last_block_size = (m_file_object.gcount() & 7) + !last_block_size * 8;

        m_bytes_returned += (data.size() - 1) * 8 + last_block_size;

        return { data, last_block_size };
    }

    static ushort GetTrashBytesCount(ushort _realSizeOfBlock)
    {
        return 8 - _realSizeOfBlock;
    }

    bool Is_EOF() const
    {
        return m_bytes_returned == m_file_size;
    }

protected:
    std::ifstream m_file_object;
    size_t m_block_size;
    size_t m_file_size;
    size_t m_bytes_returned = 0;
};

inline void WriteToFile(const std::vector<uint64_t>& data, const std::string_view filename) {
    std::ofstream file(filename, std::ios::binary);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(data.data()), data.size() * sizeof(uint64_t));
        file.close();
    }
}

void test_reader()
{
    //auto file = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/text.txt");
    //auto file = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/canterbury_last/alice29.txt");
    auto file = FileReader("/Users/full-hat/Documents/MyProjects/FHCrypto/canterbury_last/kennedy.xls");

    auto r = file.ReadBlock();
    WriteToFile(r.first, "/Users/full-hat/Documents/MyProjects/FHCrypto/canterbury_last/a.txt");
}

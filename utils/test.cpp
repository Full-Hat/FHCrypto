#include <gtest/gtest.h>

#include "file.h"

class FileReaderTest : public ::testing::Test 
{
protected:
    void SetUp() override 
    {
        input_files = {
            "/Users/full-hat/Documents/MyProjects/FHCrypto/28147-89-CFB-mode/text.txt",
            "/Users/full-hat/Documents/MyProjects/FHCrypto/Texts/alice29.txt",
            "/Users/full-hat/Documents/MyProjects/FHCrypto/Texts/asyoulik.txt"
        };
        output_file = "/Users/full-hat/Documents/MyProjects/FHCrypto/Texts/a.txt";
    }

    std::vector<std::string> input_files;
    std::string output_file;
};

TEST_F(FileReaderTest, ReadBlock8Test) 
{
    for (const auto& input_file : input_files) 
    {
        FileReader file(input_file);
        file.InitReader(std::numeric_limits<size_t>::max());

        auto r = file.ReadBlock_8();
        WriteToFile(r, output_file);

        ASSERT_TRUE(IsEqual(input_file, output_file));
    }
}

TEST_F(FileReaderTest, ReadBlock64Test) 
{
    for (const auto& input_file : input_files) 
    {
        FileReader file(input_file);
        file.InitReader(std::numeric_limits<size_t>::max());

        auto r = file.ReadBlock_64();
        WriteToFile(r.first, output_file, r.second);

        ASSERT_TRUE(IsEqual(input_file, output_file));
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <_types/_uint8_t.h>
#include <gtest/gtest.h>
#include <string>

#include "file.h"
#include "CFB.h"
#include "CTR.h"

class STB_Test : public ::testing::Test {
protected:
    void SetUp() override {
        key = { "MyVeryGoodKeyForEncryptionSomeS" };
        input_file = "/Users/full-hat/Documents/MyProjects/FHCrypto/STB-34-101-31/test.txt";
        output_file = "/Users/full-hat/Documents/MyProjects/FHCrypto/STB-34-101-31/decrypted.txt";
    }

    std::array<uint8_t, 32> key;
    std::string input_file;
    std::string output_file;
};

TEST_F(STB_Test, CFB)
{
    FileReader reader(input_file);

    STB_34_101_31_CFB crypter(key);

    auto encrypted = crypter.EncryptCFB(reader.ReadBlock_8());
    auto decrypted = crypter.DecryptCFB(encrypted);
    WriteToFile(decrypted, output_file);
    
    EXPECT_TRUE(IsEqual(input_file, output_file));
}

TEST_F(STB_Test, CTR)
{
    FileReader reader(input_file);

    STB_34_101_31_CTR crypter(key);

    auto encrypted = crypter.Encrypt(reader.ReadBlock_8());
    auto decrypted = crypter.Decrypt(encrypted);
    WriteToFile(decrypted, output_file);
    
    EXPECT_TRUE(IsEqual(input_file, output_file));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <gtest/gtest.h>

#include "ECB.h"
#include "CFB.h"

// ECB ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
TEST(GOST_28147_89_ECB_Test, EncryptionDecryptionTest) 
{
    std::array<char, 32> key { "MyVeryGoodKeyForEncryptionSomeS" };
    GOST_28147_89_ECB cryptor(key);
    int originalText = 1;
    auto encrypted = cryptor.Encrypt(originalText);
    auto decrypted = cryptor.Decrypt(encrypted);

    ASSERT_NE(originalText, encrypted);
    ASSERT_EQ(originalText, decrypted);
}

// CFB ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
class GOST_28147_89_CFB_Test : public ::testing::Test {
protected:
    void SetUp() override {
        key = { "MyVeryGoodKeyForEncryptionSomeS" };
    }

    std::array<char, 32> key;
};

TEST_F(GOST_28147_89_CFB_Test, EncryptionDecryptionTest1) {
    GOST_28147_89_CFB crypter(key);
    std::vector<uint64_t> data { 1, 1, 110 };
    auto crypted = crypter.Encrypt(data, 0);
    auto decrypted = crypter.Decrypt(crypted, 0);

    ASSERT_EQ(data, decrypted);
    ASSERT_NE(data, crypted); // Check that encrypted text is not equal to the original
}

TEST_F(GOST_28147_89_CFB_Test, EncryptionDecryptionTest2) {
    GOST_28147_89_CFB crypter(key);
    std::vector<uint64_t> emptyData;
    auto crypted = crypter.Encrypt(emptyData, 0);
    auto decrypted = crypter.Decrypt(crypted, 0);

    ASSERT_EQ(emptyData, decrypted);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

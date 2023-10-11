#include <gtest/gtest.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <array>
#include <stdexcept>

std::array<char, 128> buffer;
std::string result;

std::string exec(const std::string& command) {
    char buffer[128];
    std::string result = "";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer, sizeof buffer, pipe) != NULL) {
        result += buffer;
    }

    int status = pclose(pipe);
    if (status == -1) {
        throw std::runtime_error("pclose() failed!");
    } else if (WIFEXITED(status)) {
        int exit_status = WEXITSTATUS(status);
        if (exit_status != 0) {
            throw std::runtime_error("Command exited with non-zero status: " + std::to_string(exit_status));
        }
    } else if (WIFSIGNALED(status)) {
        int signal = WTERMSIG(status);
        throw std::runtime_error("Command was terminated by signal: " + std::to_string(signal));
    }

    return result;
}

TEST(Lab1, Task5_positive)
{
    std::string response = exec("/Users/full-hat/Documents/MyProjects/FHCrypto/build/Test15 /Users/full-hat/Documents/MyProjects/FHCrypto/Lab1 .txt");
    ASSERT_EQ(response, "/Users/full-hat/Documents/MyProjects/FHCrypto/Lab1/Test1/text.txt\n/Users/full-hat/Documents/MyProjects/FHCrypto/Lab1/text.txt\n");

    ASSERT_THROW(exec("/Users/full-hat/Documents/MyProjects/FHCrypto/build/Test15 /Users/full-hat/Documents/MyProjects/FHCrypto/Lab1 *"), std::runtime_error);
    ASSERT_THROW(exec("/Users/full-hat/Documents/MyProjects/FHCrypto/build/Test15 skclj jk"), std::runtime_error);
}

int main(int argc, char **argv)
{


    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

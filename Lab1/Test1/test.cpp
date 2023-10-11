#include "main.h"

#include <gtest/gtest.h>
#include <sstream>
#include <ranges>

#include <sstream>
#include <vector>
#include <string>

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter)) {
        result.push_back(token);
    }

    return result;
}

TEST(Task1, Lab1)
{
    std::ostringstream str;
    for (int i = 0; i < 1000; ++i)
    {
        WriteMessage(str);

        std::string string = str.str();
        auto rows = splitString(string, '\n');
        ASSERT_EQ(rows.size(), 3);
        ASSERT_EQ("Hello, world!", rows[0]);
        ASSERT_EQ("Andhiagain!", rows[1]);
        ASSERT_LE(rows[2].size(), 50);
        ASSERT_GE(rows[2].size(), 5);
        ASSERT_EQ(rows[2], std::string(rows[2].size(), '!'));

        str.str("");
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

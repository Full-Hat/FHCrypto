#include <gtest/gtest.h>
#include <stdexcept>

#include "main.h"

TEST(Lab1, Task3_positive)
{
    std::string inputs[][2] = {
        {"0.5", "0.6"},
        {"1", "1"},
        {"1", "3"},
        {"3", "1"}
    };
    std::string expectedResults[] = {
        "0.3",
        "1",
        "3",
        "3"
    };

    for (int i = 0; i < 4; ++i) {
        std::stringstream input;
        input << inputs[i][0] << std::endl << inputs[i][1];

        std::stringstream output;
        calculate_and_print(input, output);

        std::string actualResult = output.str();
        std::string expectedResult = "Area of a square: " + expectedResults[i];

        ASSERT_EQ(actualResult, expectedResult);
    }
}

TEST(Lab1, Task3_width)
{
    std::string invalidWidths[] = {
        "test",
        "0;5"
    };

    for (const auto& width : invalidWidths) {
        std::stringstream input;
        input << width;

        std::stringstream output;

        ASSERT_THROW(calculate_and_print(input, output), std::invalid_argument);
    }
}

TEST(Lab1, Task3_height)
{
    std::string invalidHeights[] = {
        "test",
        "0;5"
    };

    for (const auto& height : invalidHeights) {
        std::stringstream input;
        input << "1.5" << std::endl << height;

        std::stringstream output;
        
        ASSERT_THROW(calculate_and_print(input, output), std::invalid_argument);
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include "main.h"

#include <gtest/gtest.h>
#include <sstream>
#include <stdexcept>

#include "main.h"

TEST(Lab1, Task2_positive)
{
    std::stringstream input;
    input << R"(2
Test1 Test2 18
Test3 Test4 24)";

    std::stringstream out;

    Person::readAndPrint(out, input);

    std::string expectedOutput = "Test1 Test2 18\nTest3 Test4 24\n18 24 21.00\n";
    ASSERT_EQ(out.str(), expectedOutput);
}

TEST(Lab2, Task2_not_enough)
{
    std::stringstream input;
    input << R"(1
Test1 Test2)";

    std::stringstream out;

    ASSERT_THROW(Person::readAndPrint(out, input), std::invalid_argument);
}

TEST(Lab2, Task2_invalid_format)
{
    std::stringstream input;
    input << R"(1
Test1 Test2 15.1)";

    std::stringstream out;

    ASSERT_THROW(Person::readAndPrint(out, input), std::invalid_argument);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

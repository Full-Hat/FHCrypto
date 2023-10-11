#include <gtest/gtest.h>

#include <string>
#include <stdexcept>

#include "main.h"

// Include the file containing the generateTable and generateSingleRow functions
// #include "HtmlGenerator.h"

TEST(HtmlGeneratorTest, TestZeroRows) {
    EXPECT_THROW(generateTable(0), std::invalid_argument);
}

TEST(HtmlGeneratorTest, TestSingleRow) {
    std::string expected = R"(<!DOCTYPE html>
<html>
<head>
<style>
table {
width: 100%;
border-collapse: collapse;
}
td {
height: 50px;
}
</style>
</head>
<body>
<table>
<tr style="background: linear-gradient(to bottom, rgb(255, 255, 255), rgb(0, 0, 0)); color: rgb(0, 0, 0);">
<td>Row 1</td>
</tr>
</table>
</body>
</html>
)";

    std::string actual = generateTable(1);

    EXPECT_EQ(expected, actual);
}

TEST(HtmlGeneratorTest, TestTwoRows) {
    std::string expected = R"(<!DOCTYPE html>
<html>
<head>
<style>
table {
width: 100%;
border-collapse: collapse;
}
td {
height: 50px;
}
</style>
</head>
<body>
<table>
<tr style="background: linear-gradient(to bottom, rgb(255, 255, 255), rgb(128, 128, 128)); color: rgb(0, 0, 0);">
<td>Row 1</td>
</tr><tr style="background: linear-gradient(to bottom, rgb(128, 128, 128), rgb(1, 1, 1)); color: rgb(63, 63, 63);">
<td>Row 2</td>
</tr>
</table>
</body>
</html>
)";

    std::string actual = generateTable(2);

    EXPECT_EQ(expected, actual);
}

TEST(HtmlGeneratorTest, TestSingleRowGeneration) {
    std::vector<std::tuple<int, int, int, int, int>> testCases = {
        {0, 1, 255, 0, 0},
        {0, 2, 255, 128, 0},
        {1, 2, 128, 1, 63}
    };

    for (const auto& testCase : testCases) {
        int rowIndex, totalRowCount, expectedHigh, expectedLow, expectedForeground;
        std::tie(rowIndex, totalRowCount, expectedHigh, expectedLow, expectedForeground) = testCase;

        std::stringstream expected;
        expected << "<tr style=\"background: linear-gradient(to bottom, rgb(" << expectedHigh << ", " << expectedHigh << ", " << expectedHigh << "), rgb(" << expectedLow << ", " << expectedLow << ", " << expectedLow << ")); color: rgb(" << expectedForeground << ", " << expectedForeground << ", " << expectedForeground << ");\">\n<td>Row " << (rowIndex + 1) << "</td>\n</tr>";

        std::string row = generateSingleRow(rowIndex, totalRowCount);

        EXPECT_EQ(expected.str(), row);
    }
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

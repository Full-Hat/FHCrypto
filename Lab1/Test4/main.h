#include <string>
#include <sstream>

const std::string MainTemplate = R"(<!DOCTYPE html>
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
{0}
</table>
</body>
</html>
)";

const std::string RowTemplate = R"(
<tr style="background: linear-gradient(to bottom, rgb({0}, {0}, {0}), rgb({1}, {1}, {1})); color: rgb({2}, {2}, {2});">
<td>Row {3}</td></tr>)";

std::string generateSingleRow(int rowIndex, int totalRowCount);

std::string generateTable(int rows) {
    if (rows <= 0) {
        throw std::invalid_argument("Rows must be greater than 0");
    }

    std::stringstream rowBuilder;

    for (int i = 0; i < rows; i++) {
        rowBuilder << generateSingleRow(i, rows);
    }

    std::string mainTemplate = MainTemplate;
    std::string rowContent = rowBuilder.str();
    size_t pos = mainTemplate.find("{0}");
    if (pos != std::string::npos) {
        mainTemplate.replace(pos, 3, rowContent);
    }

    return mainTemplate;
}

std::string generateSingleRow(int rowIndex, int totalRowCount) {
    int step = 255 / totalRowCount;

    int high = 255 - step * rowIndex;
    int low = 255 - step * (rowIndex + 1);

    int foreground = ((high - low) * rowIndex) / 2 * rowIndex;

    std::stringstream row;
    row << "<tr style=\"background: linear-gradient(to bottom, rgb(" << high << ", " << high << ", " << high << "), rgb(" << low << ", " << low << ", " << low << ")); color: rgb(" << foreground << ", " << foreground << ", " << foreground << ");\">\n<td>Row " << (rowIndex + 1) << "</td>\n</tr>";

    return row.str();
}

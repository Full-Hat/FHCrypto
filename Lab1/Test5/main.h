#include <filesystem>
#include <vector>

std::vector<std::string> FindFiles(const std::string& basePath, const std::string& extension) {
    std::vector<std::string> result;

    for (const auto& entry : std::filesystem::recursive_directory_iterator(basePath)) {
        if (entry.is_regular_file() && entry.path().extension() == extension) {
            result.push_back(entry.path().string());
        }
    }

    return result;
}

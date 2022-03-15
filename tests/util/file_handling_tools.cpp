#include <fstream>
#include <iostream>
#include <string>

#include "file_handling_tools.hpp"

void
mizcore::read_json_file(nlohmann::json& json, const std::filesystem::path& path)
{
    std::ifstream ifs(path);
    ifs >> json;
}

void
mizcore::write_json_file(const nlohmann::json& json,
                         const std::filesystem::path& path)
{
    std::ofstream ofs(path);
    ofs << json.dump() << std::endl;
}

nlohmann::json
mizcore::json_file_diff(const std::filesystem::path& path1,
                        const std::filesystem::path& path2)
{
    nlohmann::json j1;
    nlohmann::json j2;

    read_json_file(j1, path1);
    read_json_file(j2, path2);

    return nlohmann::json::diff(j1, j2);
}

bool
mizcore::check_text_diff(const std::filesystem::path& path1,
                         const std::filesystem::path& path2)
{
    bool diff_found = false;

    std::ifstream ifs1(path1);
    std::ifstream ifs2(path2);

    std::string line1;
    std::string line2;
    for (size_t i = 1;; ++i) {
        line1.clear();
        line2.clear();
        std::getline(ifs1, line1);
        std::getline(ifs2, line2);
        if (line1.empty() && line2.empty()) {
            break;
        }

        if (line1 != line2) {
            diff_found = true;
            std::cout << "Diff found: line = " << i << ":\n"
                      << line1 << "\n"
                      << line2 << std::endl;
        }
    }

    return diff_found;
}

#pragma once

#include "nlohmann/json.hpp"
#include <filesystem>

namespace mizcore {

void
read_json_file(nlohmann::json& json, const std::filesystem::path& path);

void
write_json_file(const nlohmann::json& json, const std::filesystem::path& path);

nlohmann::json
json_file_diff(const std::filesystem::path& path1,
               const std::filesystem::path& path2);

bool
check_text_diff(const std::filesystem::path& path1,
                const std::filesystem::path& path2);

} // namespace mizcore
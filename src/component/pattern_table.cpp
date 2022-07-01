#include "pattern_table.hpp"
#include "pattern_element.hpp"
#include <string_view>

using mizcore::PatternTable;
using json = nlohmann::json;

void
PatternTable::AddPatternElement(std::shared_ptr<PatternElement>&& pattern)
{
    std::string filename = std::string(pattern->GetFilename());
    auto it = file2patterns_.find(filename);
    if (it == file2patterns_.end()) {
        PatternElements patterns = { std::move(pattern) };
        file2patterns_[filename] = std::move(patterns);
    } else {
        it->second.emplace_back(std::move(pattern));
    }
}

void
PatternTable::ToJson(nlohmann::json& json) const
{
    for (const auto& [filename, patterns] : file2patterns_) {
        nlohmann::json j;
        for (const auto& pattern : patterns) {
            pattern->ToJson(j);
            json.push_back(j);
        }
    }
}

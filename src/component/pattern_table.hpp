#pragma once

#include <map>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"
#include "pattern_element.hpp"

namespace mizcore {

class PatternElement;

class PatternTable
{
  public:
    // ctor, dtor
    PatternTable() = default;
    virtual ~PatternTable() = default;

    PatternTable(const PatternTable&) = delete;
    PatternTable(PatternTable&&) = delete;
    PatternTable& operator=(const PatternTable&) = delete;
    PatternTable& operator=(PatternTable&&) = delete;

    // attributes
    void AddPatternElement(std::shared_ptr<PatternElement>&& pattern);

    // operations
    void ToJson(nlohmann::json& json) const;

  private:
    // implementation
    using PatternElements = std::vector<std::shared_ptr<PatternElement>>;
    std::map<std::string, PatternElements> file2patterns_;
};

} // namespace mizcore

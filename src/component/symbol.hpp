#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "ast_type.hpp"

namespace mizcore {

class Symbol final
{
  public:
    Symbol(std::string_view text, SYMBOL_TYPE type, uint8_t priority = 64);
    std::string_view GetText() const { return text_; }
    SYMBOL_TYPE GetType() const { return type_; }
    uint8_t GetPriority() const { return priority_; }
    std::string_view GetTypeString() const;
    SPECIAL_SYMBOL_TYPE GetSpecialType() const { return special_type_; }

  private:
    std::string text_;
    SYMBOL_TYPE type_;
    uint8_t priority_;
    SPECIAL_SYMBOL_TYPE special_type_;
};

} // namespace mizcore

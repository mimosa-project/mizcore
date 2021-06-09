#pragma once

#include <cstdint>
#include <string>
#include <string_view>

#include "symbol_type.hpp"

namespace mizcore {

class Symbol final
{
  public:
    Symbol(std::string_view text, SYMBOL_TYPE type, uint8_t priority = 64)
      : text_(text)
      , type_(type)
      , priority_(priority)
    {}

    std::string_view GetText() const { return text_; }
    SYMBOL_TYPE GetType() const { return type_; }
    uint8_t GetPriority() const { return priority_; }
    std::string_view GetTypeString() const;

  private:
    std::string text_;
    SYMBOL_TYPE type_;
    uint8_t priority_;
};

} // namespace mizcore

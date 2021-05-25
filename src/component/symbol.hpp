#pragma once

#include <cstdint>
#include <string>

namespace emcore {

// R - Predicate,
// O - Functor,
// M - Mode,
// G - Structure,
// U - Selector,
// V - Attribute,
// K - Left Functor Bracket,
// L - Right Functor Bracket.
enum class SYMBOL_TYPE : char
{
    UNKNOWN = '-',
    PREDICATE = 'R',
    FUNCTOR = 'O',
    MODE = 'M',
    STRUCTURE = 'G',
    SELECTOR = 'U',
    ATTRIBUTE = 'V',
    LEFT_FUNCTOR_BRACKET = 'K',
    RIGHT_FUNCTOR_BRACKET = 'L',
    SPECIAL = 'S',
};

class Symbol final
{
  public:
    Symbol(std::string&& text, SYMBOL_TYPE type, uint8_t priority = 64)
      : text_(text)
      , type_(type)
      , priority_(priority)
    {}
    ~Symbol() = default;

    const char* GetText() const { return text_.c_str(); }
    size_t GetLength() const { return text_.length(); }
    SYMBOL_TYPE GetType() const { return type_; }
    uint8_t GetPriority() const { return priority_; }

  private:
    std::string text_;
    SYMBOL_TYPE type_;
    uint8_t priority_;
};

} // namespace emcore

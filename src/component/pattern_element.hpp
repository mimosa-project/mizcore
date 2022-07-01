#pragma once

#include "ast_token.hpp"
#include "ast_type.hpp"
#include <array>
#include <string>

namespace mizcore {

class SymbolToken;

class PatternElement
{
  public:
    // ctor, dtor
    PatternElement(std::string_view filename)
      : filename_(filename)
    {}
    virtual ~PatternElement() = default;

    PatternElement(const PatternElement&) = delete;
    PatternElement(PatternElement&&) = delete;
    PatternElement& operator=(const PatternElement&) = delete;
    PatternElement& operator=(PatternElement&&) = delete;

    void SetFunctorPattern(SymbolToken* symbol_token,
                           size_t left_arity,
                           size_t right_arity);
    void SetBracketFunctorPattern(SymbolToken* left_symbol_token,
                                  SymbolToken* right_symbol_token,
                                  size_t arity);
    void SetAttributePattern(SymbolToken* symbol_token, size_t arity);
    void SetModePattern(SymbolToken* symbol_token, size_t arity);
    void SetPredicatePattern(SymbolToken* symbol_token,
                             size_t left_arity,
                             size_t right_arity);

    SymbolToken* GetSymbolToken() const { return symbol_tokens_[0]; }
    SymbolToken* GetLeftSymbolToken() const { return symbol_tokens_[0]; }
    SymbolToken* GetRightSymbolToken() const { return symbol_tokens_[1]; }
    std::string_view GetFilename() const { return filename_; }
    uint8_t GetArity() const { return arities_[0]; }
    uint8_t GetLeftArity() const { return arities_[0]; }
    uint8_t GetRightArity() const { return arities_[1]; }

    // operations
    void ToJson(nlohmann::json& json) const;
    bool operator<(const PatternElement& other) const;

  private:
    PATTERN_TYPE type_ = PATTERN_TYPE::UNKNOWN;
    std::string filename_;
    std::array<SymbolToken*, 2> symbol_tokens_ = { nullptr, nullptr };
    std::array<uint8_t, 2> arities_ = { 0, 0 };
};

} // namespace mizcore
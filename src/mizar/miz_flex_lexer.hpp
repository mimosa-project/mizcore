#pragma once

#include <memory>
#include <stack>

#include "token_type.hpp"

namespace emcore::mizar {

class SymbolTable;
class Token;
class TokenArray;
using std::istream;

class MizFlexLexer : public yyMizFlexLexer
{
  public:
    MizFlexLexer(std::istream* in, std::shared_ptr<SymbolTable> symbol_table);
    virtual ~MizFlexLexer() = default;

    virtual int yylex();

    std::shared_ptr<TokenArray> GetTokenArray() const { return token_array_; }

  private:
    size_t ScanSymbol();
    size_t ScanIdentifier();
    size_t ScanKeyword(TOKEN_TYPE token_type);
    size_t ScanNumeral();
    size_t ScanFileName();
    size_t ScanComment(TOKEN_TYPE token_type);
    size_t ScanUnknown();

  private:
    std::shared_ptr<SymbolTable> symbol_table_;
    std::shared_ptr<TokenArray> token_array_;
    size_t line_number_;
    size_t column_number_;

    bool is_in_environ_section_;
    bool is_in_vocabulary_section_;
};

} // namespace emcore::mizar

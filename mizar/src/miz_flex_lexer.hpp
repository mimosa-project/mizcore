#pragma once

#include <memory>
#include <stack>

#include "token_type.hpp"

namespace emcore::mizar {

class SymbolTable;
class Token;
class TokenFactory;
class TokenArray;
using std::istream;

class MizFlexLexer : public yyMizFlexLexer {
public:
    MizFlexLexer(std::istream *in, std::shared_ptr<SymbolTable> symbol_table);
    virtual ~MizFlexLexer() = default;

    virtual int yylex();

    std::shared_ptr<TokenArray> GetTokenArray() const {return token_array_;}

private:
    size_t ScanSymbol();
    size_t ScanVariable();
    size_t ScanKeyword(KEYWORD_TYPE keyword_type);
    size_t ScanNumeral();
    size_t ScanFileName();
    size_t ScanComment(COMMENT_TYPE comment_type);
    size_t ScanUnknown();

private:
    std::shared_ptr<SymbolTable> symbol_table_;
    std::shared_ptr<TokenFactory> token_factory_;
    std::shared_ptr<TokenArray> token_array_;
    size_t line_number_;
    size_t column_number_;
};

}

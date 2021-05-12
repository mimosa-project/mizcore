#pragma once

#include <memory>
#include "token_type.hpp"

namespace emcore::mizar {

class Token;
class SymbolTable;

class TokenFactory {
public:
    // ctor, dtor
    TokenFactory(std::shared_ptr<SymbolTable> symbol_table)
        : symbol_table_(symbol_table) {}
    virtual ~TokenFactory() = default;

    // operation
    Token* CreateSymbolToken(size_t line_number, size_t column_number, const char* text);
    Token* CreateIdentifierToken(size_t line_number, size_t column_number, const char* text);
    Token* CreateKeywordToken(size_t line_number, size_t column_number, KEYWORD_TYPE keyword_type);
    Token* CreateNumeralToken(size_t line_number, size_t column_number, const char* text);
    Token* CreateFileNameToken(size_t line_number, size_t column_number, const char* text);
    Token* CreateCommentToken(size_t line_number, size_t column_number, const char* text, COMMENT_TYPE comment_type);
    Token* CreateUnknownToken(size_t line_number, size_t column_number, const char* text);
private:
    std::shared_ptr<SymbolTable> symbol_table_;
};

}

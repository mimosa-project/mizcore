#pragma once

#include <string>
#include <memory>

#include "token_type.hpp"

namespace emcore::mizar {

class Symbol;

class Token final {
public:
    // ctor, dtor
    Token(size_t line_number, size_t column_number, TOKEN_TYPE type,
          const char* text = nullptr, Symbol* symbol = nullptr);
    Token(const Token &rhs);
    Token(Token&& rhs);
    Token& operator=(const Token& rhs);
    Token& operator=(Token&& rhs);
    ~Token();

    // attributes
    int GetLineNumber() const {return line_number_;}
    int GetColumnNumber() const {return column_number_;}
    const char* GetText() const;
    size_t GetLength() const;
    TOKEN_TYPE GetType() const {return type_;}

    // operations
    void Dump(std::ostream& os) const;

    // static
    static const char* QueryTypeText(TOKEN_TYPE type);
    static TOKEN_TYPE QueryKeywordType(const char* text);
private:
    size_t line_number_;
    size_t column_number_;
    TOKEN_TYPE type_;
    union {
        char* text_;
        Symbol* symbol_;
    };
};

}

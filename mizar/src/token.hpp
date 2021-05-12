#pragma once

#include <string>
#include <memory>

#include "token_type.hpp"

namespace emcore::mizar {

class Symbol;

class Token {
public:
    // ctor, dtor
    Token(size_t line_number, size_t column_number)
      : line_number_(line_number), column_number_(column_number) {}
    virtual ~Token() = default;

    // attributes
    int GetLineNumber() const {return line_number_;}
    int GetColumnNumber() const {return column_number_;}

    // override
    virtual TOKEN_TYPE GetType() const = 0;
    virtual size_t GetLength() const = 0;
    virtual const std::string& GetText() const = 0;
    virtual void Dump(std::ostream& os) const;

    // static
    static const char* QueryTypeText(TOKEN_TYPE type);
private:
    size_t line_number_;
    size_t column_number_;
};

class SymbolToken : public Token {
public:
    // ctor, dtor
    SymbolToken(size_t line_number, size_t column_number, Symbol* symbol)
      : Token(line_number, column_number), symbol_(symbol) {}
    ~SymbolToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::SYMBOL;}
    size_t GetLength() const override;
    const std::string& GetText() const override;
    void Dump(std::ostream& os) const override;
private:
    Symbol* symbol_;
};

class VariableToken : public Token {
public:
    // ctor, dtor
    VariableToken(size_t line_number, size_t column_number, const char* text)
      : Token(line_number, column_number), text_(text), ref_(nullptr) {}
    ~VariableToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::VARIABLE;}
    size_t GetLength() const override {return text_.length();}
    const std::string& GetText() const override {return text_;}
private:
    std::string text_;
    VariableToken* ref_;
};

class KeywordToken : public Token {
public:
    // ctor, dtor
    KeywordToken(size_t line_number, size_t column_number, KEYWORD_TYPE keyword_type)
      : Token(line_number, column_number), keyword_type_(keyword_type) {}
    ~KeywordToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::KEYWORD;}
    size_t GetLength() const override;
    const std::string& GetText() const override;

    // attribute
    KEYWORD_TYPE GetKeywordType() const {return keyword_type_;}

    // static function
    static KEYWORD_TYPE QueryKeywordType(const char* name);
    static const std::string& QueryKeywordText(KEYWORD_TYPE keyword_type);
private:
    KEYWORD_TYPE keyword_type_;
};

class NumeralToken : public Token {
public:
    // ctor, dtor
    NumeralToken(size_t line_number, size_t column_number, const char* text)
        : Token(line_number, column_number), text_(text) {}
    ~NumeralToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::NUMERAL;}
    size_t GetLength() const override {return text_.size();}
    const std::string& GetText() const override {return text_;}
private:
    std::string text_;
};

class FileNameToken : public Token {
public:
    // ctor, dtor
    FileNameToken(size_t line_number, size_t column_number, const char* text)
      : Token(line_number, column_number), text_(text) {}
    ~FileNameToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::FILENAME;}
    size_t GetLength() const override {return text_.size();}
    const std::string& GetText() const override {return text_;}
private:
    std::string text_;
};

class CommentToken : public Token {
public:
    // ctor, dtor
    CommentToken(size_t line_number, size_t column_number, const char* text, COMMENT_TYPE comment_type)
      : Token(line_number, column_number), text_(text), comment_type_(comment_type) {}
    ~CommentToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::COMMENT;}
    size_t GetLength() const override {return text_.size();}
    const std::string& GetText() const override {return text_;}

    // attribute
    COMMENT_TYPE GetCommentType() const {return comment_type_;}
private:
    std::string text_;
    COMMENT_TYPE comment_type_;
};

class UnknownToken : public Token {
public:
    // ctor, dtor
    UnknownToken(size_t line_number, size_t column_number, const char* text)
      : Token(line_number, column_number), text_(text) {}
    ~UnknownToken() override = default;

    // override
    TOKEN_TYPE GetType() const override {return TOKEN_TYPE::UNKNOWN;}
    size_t GetLength() const override {return text_.size();}
    const std::string& GetText() const override {return text_;}
private:
    std::string text_;
};

}

#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "nlohmann/json.hpp"
#include "token_type.hpp"

namespace mizcore {

class Symbol;

class Token
{
  public:
    // ctor, dtor
    Token(size_t line_number, size_t column_number)
      : line_number_(line_number)
      , column_number_(column_number)
    {}
    virtual ~Token() = default;

    Token(const Token&) = delete;
    Token(Token&&) = delete;
    Token& operator=(const Token&) = delete;
    Token& operator=(Token&&) = delete;

    // attributes
    size_t GetId() const { return id_; }
    void SetId(size_t id) { id_ = id; }
    int GetLineNumber() const { return line_number_; }
    int GetColumnNumber() const { return column_number_; }
    virtual std::string_view GetText() const = 0;
    virtual TOKEN_TYPE GetTokenType() const = 0;

    // operations
    virtual void ToJson(nlohmann::json& json) const;

  private:
    size_t id_ = SIZE_MAX;
    size_t line_number_;
    size_t column_number_;
};

class UnknownToken : public Token
{
  public:
    // ctor, dtor
    UnknownToken(size_t line_number,
                 size_t column_number,
                 std::string_view text)
      : Token(line_number, column_number)
      , text_(text)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    void AddText(std::string_view s) { text_ += s; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::UNKNOWN; }

  private:
    std::string text_;
};

class NumeralToken : public Token
{
  public:
    // ctor, dtor
    NumeralToken(size_t line_number,
                 size_t column_number,
                 std::string_view text)
      : Token(line_number, column_number)
      , text_(text)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::NUMERAL; }

  private:
    std::string text_;
};

class SymbolToken : public Token
{
  public:
    // ctor, dtor
    SymbolToken(size_t line_number, size_t column_number, Symbol* symbol)
      : Token(line_number, column_number)
      , symbol_(symbol)
    {}

    // attributes
    std::string_view GetText() const override;
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::SYMBOL; }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    Symbol* symbol_;
};

class IdentifierToken : public Token
{
  public:
    // ctor, dtor
    IdentifierToken(size_t line_number,
                    size_t column_number,
                    std::string_view text,
                    IDENTIFIER_TYPE identifier_type = IDENTIFIER_TYPE::UNKNOWN)
      : Token(line_number, column_number)
      , text_(text)
      , identifier_type_(identifier_type)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::IDENTIFIER; }
    IDENTIFIER_TYPE GetIdentifierType() const { return identifier_type_; }
    void SetIdentifierType(IDENTIFIER_TYPE identifier_type)
    {
        identifier_type_ = identifier_type;
    }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    std::string text_;
    IDENTIFIER_TYPE identifier_type_;
};

class CommentToken : public Token
{
  public:
    // ctor, dtor
    CommentToken(size_t line_number,
                 size_t column_number,
                 std::string_view text,
                 COMMENT_TYPE comment_type = COMMENT_TYPE::UNKNOWN)
      : Token(line_number, column_number)
      , text_(text)
      , comment_type_(comment_type)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::COMMENT; }
    COMMENT_TYPE GetCommentType() const { return comment_type_; }
    void SetCommentType(COMMENT_TYPE comment_type)
    {
        comment_type_ = comment_type;
    }

  private:
    std::string text_;
    COMMENT_TYPE comment_type_;
};

class KeywordToken : public Token
{
  public:
    // ctor, dtor
    KeywordToken(size_t line_number,
                 size_t column_number,
                 KEYWORD_TYPE keyword_type = KEYWORD_TYPE::UNKNOWN)
      : Token(line_number, column_number)
      , keyword_type_(keyword_type)
    {}

    // attributes
    std::string_view GetText() const override
    {
        return QueryKeywordText(keyword_type_);
    }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::KEYWORD; }

    KEYWORD_TYPE GetKeywordType() const { return keyword_type_; }
    void SetKeywordType(KEYWORD_TYPE keyword_type)
    {
        keyword_type_ = keyword_type;
    }

  private:
    KEYWORD_TYPE keyword_type_;
};

} // namespace mizcore

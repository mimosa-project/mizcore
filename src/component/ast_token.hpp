#pragma once

#include <memory>
#include <string>
#include <string_view>

#include "ast_element.hpp"
#include "ast_type.hpp"
#include "nlohmann/json.hpp"

namespace mizcore {

class Symbol;
class IdentifierToken;

class ASTToken : public ASTElement
{
  public:
    // ctor, dtor
    ASTToken(size_t line_number, size_t column_number)
      : line_number_(line_number)
      , column_number_(column_number)
    {}
    ~ASTToken() override = default;

    ASTToken(const ASTToken&) = delete;
    ASTToken(ASTToken&&) = delete;
    ASTToken& operator=(const ASTToken&) = delete;
    ASTToken& operator=(ASTToken&&) = delete;

    // attributes
    ELEMENT_TYPE GetElementType() const override { return ELEMENT_TYPE::TOKEN; }
    size_t GetId() const { return id_; }
    void SetId(size_t id) { id_ = id; }
    int GetLineNumber() const { return line_number_; }
    int GetColumnNumber() const { return column_number_; }
    virtual std::string_view GetText() const = 0;
    virtual TOKEN_TYPE GetTokenType() const = 0;
    virtual IdentifierToken* GetRefToken() const = 0;

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    size_t id_ = SIZE_MAX;
    size_t line_number_;
    size_t column_number_;
};

class UnknownToken : public ASTToken
{
  public:
    // ctor, dtor
    UnknownToken(size_t line_number,
                 size_t column_number,
                 std::string_view text)
      : ASTToken(line_number, column_number)
      , text_(text)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    void AddText(std::string_view s) { text_ += s; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::UNKNOWN; }
    IdentifierToken* GetRefToken() const override { return nullptr; }

  private:
    std::string text_;
};

class NumeralToken : public ASTToken
{
  public:
    // ctor, dtor
    NumeralToken(size_t line_number,
                 size_t column_number,
                 std::string_view text)
      : ASTToken(line_number, column_number)
      , text_(text)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::NUMERAL; }
    IdentifierToken* GetRefToken() const override { return nullptr; }

  private:
    std::string text_;
};

class SymbolToken : public ASTToken
{
  public:
    // ctor, dtor
    SymbolToken(size_t line_number, size_t column_number, Symbol* symbol)
      : ASTToken(line_number, column_number)
      , symbol_(symbol)
    {}

    // attributes
    std::string_view GetText() const override;
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::SYMBOL; }
    SYMBOL_TYPE GetSymbolType() const;
    SPECIAL_SYMBOL_TYPE GetSpecialSymbolType() const;
    IdentifierToken* GetRefToken() const override { return nullptr; }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    Symbol* symbol_;
};

class IdentifierToken : public ASTToken
{
  public:
    // ctor, dtor
    IdentifierToken(size_t line_number,
                    size_t column_number,
                    std::string_view text,
                    IDENTIFIER_TYPE identifier_type = IDENTIFIER_TYPE::UNKNOWN)
      : ASTToken(line_number, column_number)
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

    IdentifierToken* GetRefToken() const override { return ref_token_; }
    void SetRefToken(IdentifierToken* ref_token) { ref_token_ = ref_token; }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    std::string text_;
    IDENTIFIER_TYPE identifier_type_;
    IdentifierToken* ref_token_ = nullptr;
};

class CommentToken : public ASTToken
{
  public:
    // ctor, dtor
    CommentToken(size_t line_number,
                 size_t column_number,
                 std::string_view text,
                 COMMENT_TYPE comment_type = COMMENT_TYPE::UNKNOWN)
      : ASTToken(line_number, column_number)
      , text_(text)
      , comment_type_(comment_type)
    {}

    // attributes
    std::string_view GetText() const override { return text_; }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::COMMENT; }
    IdentifierToken* GetRefToken() const override { return nullptr; }
    COMMENT_TYPE GetCommentType() const { return comment_type_; }
    void SetCommentType(COMMENT_TYPE comment_type)
    {
        comment_type_ = comment_type;
    }

  private:
    std::string text_;
    COMMENT_TYPE comment_type_;
};

class KeywordToken : public ASTToken
{
  public:
    // ctor, dtor
    KeywordToken(size_t line_number,
                 size_t column_number,
                 KEYWORD_TYPE keyword_type = KEYWORD_TYPE::UNKNOWN)
      : ASTToken(line_number, column_number)
      , keyword_type_(keyword_type)
    {}

    // attributes
    std::string_view GetText() const override
    {
        return QueryKeywordText(keyword_type_);
    }
    TOKEN_TYPE GetTokenType() const override { return TOKEN_TYPE::KEYWORD; }
    IdentifierToken* GetRefToken() const override { return nullptr; }

    KEYWORD_TYPE GetKeywordType() const { return keyword_type_; }
    void SetKeywordType(KEYWORD_TYPE keyword_type)
    {
        keyword_type_ = keyword_type;
    }

  private:
    KEYWORD_TYPE keyword_type_;
};

} // namespace mizcore

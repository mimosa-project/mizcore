#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

#include "symbol_table.hpp"
#include "token.hpp"
#include "token_array.hpp"

#undef yyFlexLexer
#define yyFlexLexer yyMizFlexLexer
#include <FlexLexer.h>

#include "miz_flex_lexer.hpp"

using emcore::mizar::MizFlexLexer;

MizFlexLexer::MizFlexLexer(std::istream* in,
                           std::shared_ptr<SymbolTable> symbol_table)
  : yyMizFlexLexer(in)
  , symbol_table_(symbol_table)
  , token_array_(std::make_shared<TokenArray>())
  , line_number_(1)
  , column_number_(1)
  , is_in_environ_section_(false)
  , is_in_vocabulary_section_(false)
{}

size_t
MizFlexLexer::ScanSymbol()
{
    Symbol* symbol = symbol_table_->QueryLongestMatchSymbol(yytext);
    if (symbol) {
        Token* token = new Token(
          line_number_, column_number_, TOKEN_TYPE::SYMBOL, nullptr, symbol);
        token_array_->AddToken(token);
        size_t length = token->GetLength();
        column_number_ += length;

        if (0 == strcmp(token->GetText(), ";") && is_in_vocabulary_section_) {
            is_in_vocabulary_section_ = false;
        }
        return length;
    } else {
        return 0;
    }
}

size_t
MizFlexLexer::ScanIdentifier()
{
    Token* token =
      new Token(line_number_, column_number_, TOKEN_TYPE::IDENTIFIER, yytext);
    token_array_->AddToken(token);
    column_number_ += yyleng;
    return yyleng;
}

size_t
MizFlexLexer::ScanKeyword(TOKEN_TYPE token_type)
{
    Token* token = new Token(line_number_, column_number_, token_type, nullptr);
    token_array_->AddToken(token);
    column_number_ += yyleng;

    if (token_type == TOKEN_TYPE::ENVIRON) {
        is_in_environ_section_ = true;
    } else if (token_type == TOKEN_TYPE::VOCABULARIES) {
        if (is_in_environ_section_) {
            is_in_vocabulary_section_ = true;
        }
    } else if (token_type == TOKEN_TYPE::BEGIN_) {
        is_in_environ_section_ = false;
        is_in_vocabulary_section_ = false;
        symbol_table_->BuildQueryMap();
    }

    return yyleng;
}

size_t
MizFlexLexer::ScanNumeral()
{
    Token* token =
      new Token(line_number_, column_number_, TOKEN_TYPE::NUMERAL, yytext);
    assert(token);
    token_array_->AddToken(token);
    column_number_ += yyleng;
    return yyleng;
}

size_t
MizFlexLexer::ScanFileName()
{
    if (is_in_environ_section_) {
        Token* token =
          new Token(line_number_, column_number_, TOKEN_TYPE::FILENAME, yytext);
        assert(token);
        token_array_->AddToken(token);
        column_number_ += yyleng;

        if (is_in_vocabulary_section_) {
            symbol_table_->AddValidFileName(token->GetText());
        }
        return yyleng;
    } else {
        return 0;
    }
}

size_t
MizFlexLexer::ScanComment(TOKEN_TYPE token_type)
{
    Token* token = new Token(line_number_, column_number_, token_type, yytext);
    assert(token);
    token_array_->AddToken(token);
    column_number_ += yyleng;
    return yyleng;
}

size_t
MizFlexLexer::ScanUnknown()
{
    // [TODO] temporary error message
    std::cout << "[Error] Unknown token found: [" << line_number_ << ","
              << column_number_ << "]" << std::endl;
    Token* token =
      new Token(line_number_, column_number_, TOKEN_TYPE::UNKNOWN, yytext);
    assert(token);
    token_array_->AddToken(token);
    column_number_ += yyleng;
    return yyleng;
}

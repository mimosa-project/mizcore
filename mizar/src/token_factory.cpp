#include "symbol_table.hpp"
#include "token.hpp"
#include "token_factory.hpp"

using emcore::mizar::Token;
using emcore::mizar::TokenFactory;

Token* TokenFactory::CreateSymbolToken(size_t line_number, size_t column_number, const char* text)
{
    Symbol* symbol = symbol_table_->QueryLongestMatchSymbol(text);
    if (symbol) {
        return new Token(line_number, column_number, TOKEN_TYPE::SYMBOL, nullptr, symbol);
    } else {
        return nullptr;
    }
}

Token* TokenFactory::CreateNumeralToken(size_t line_number, size_t column_number, const char* text)
{
    return new Token(line_number, column_number, TOKEN_TYPE::NUMERAL, text);
}

Token* TokenFactory::CreateIdentifierToken(size_t line_number, size_t column_number, const char* text)
{
    return new Token(line_number, column_number, TOKEN_TYPE::IDENTIFIER, text);
}

Token* TokenFactory::CreateKeywordToken(size_t line_number, size_t column_number, TOKEN_TYPE token_type)
{
    return new Token(line_number, column_number, token_type, nullptr);
}


Token* TokenFactory::CreateFileNameToken(size_t line_number, size_t column_number, const char* text)
{
    return new Token(line_number, column_number, TOKEN_TYPE::FILENAME, text);
}

Token* TokenFactory::CreateCommentToken(size_t line_number, size_t column_number, const char* text, TOKEN_TYPE token_type)
{
    return new Token(line_number, column_number, token_type, text);
}

Token* TokenFactory::CreateUnknownToken(size_t line_number, size_t column_number, const char* text)
{
    return new Token(line_number, column_number, TOKEN_TYPE::UNKNOWN, text);
}

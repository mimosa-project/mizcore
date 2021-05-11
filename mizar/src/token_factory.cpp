#include "symbol_table.hpp"
#include "token.hpp"
#include "token_factory.hpp"

using emcore::mizar::Token;
using emcore::mizar::TokenFactory;

Token* TokenFactory::CreateSymbolToken(size_t line_number, size_t column_number, const char* text)
{
    Symbol* symbol = symbol_table_->QueryLongestMatchSymbol(text);
    if (symbol) {
        return new SymbolToken(line_number, column_number, symbol);
    } else {
        return nullptr;
    }
}

Token* TokenFactory::CreateVariableToken(size_t line_number, size_t column_number, const char* text)
{
    return new VariableToken(line_number, column_number, text);
}

Token* TokenFactory::CreateKeywordToken(size_t line_number, size_t column_number, KEYWORD_TYPE keyword_type)
{
    return new KeywordToken(line_number, column_number, keyword_type);
}

Token* TokenFactory::CreateNumeralToken(size_t line_number, size_t column_number, const char* text)
{
    return new NumeralToken(line_number, column_number, text);
}

Token* TokenFactory::CreateFileNameToken(size_t line_number, size_t column_number, const char* text)
{
    return new FileNameToken(line_number, column_number, text);
}

Token* TokenFactory::CreateCommentToken(size_t line_number, size_t column_number, const char* text, COMMENT_TYPE comment_type)
{
    return new CommentToken(line_number, column_number, text, comment_type);
}

Token* TokenFactory::CreateUnknownToken(size_t line_number, size_t column_number, const char* text)
{
    return new UnknownToken(line_number, column_number, text);
}

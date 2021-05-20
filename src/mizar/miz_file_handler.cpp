#include <memory>

#undef yyFlexLexer
#define yyFlexLexer yyMizFlexLexer
#include <FlexLexer.h>

#include "miz_file_handler.hpp"
#include "miz_flex_lexer.hpp"

using emcore::mizar::MizFileHandler;
using emcore::mizar::MizFlexLexer;
using emcore::mizar::SymbolTable;
using emcore::mizar::TokenArray;

MizFileHandler::MizFileHandler(std::istream* in,
                               std::shared_ptr<SymbolTable> symbol_table)
  : miz_flex_lexer_(std::make_shared<MizFlexLexer>(in, symbol_table))
{}

int
MizFileHandler::yylex()
{
    return miz_flex_lexer_->yylex();
}

std::shared_ptr<TokenArray>
MizFileHandler::GetTokenArray() const
{
    return miz_flex_lexer_->GetTokenArray();
}

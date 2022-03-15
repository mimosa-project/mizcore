#include <memory>

#undef yyFlexLexer
#define yyFlexLexer yyVctFlexLexer
#include <FlexLexer.h>

#include "vct_flex_lexer.hpp"
#include "vct_lexer_handler.hpp"

using mizcore::SymbolTable;
using mizcore::VctFlexLexer;
using mizcore::VctLexerHandler;

VctLexerHandler::VctLexerHandler(std::istream* in)
  : vct_flex_lexer_(std::make_shared<VctFlexLexer>(in))
{}

int
VctLexerHandler::yylex()
{
    return vct_flex_lexer_->yylex();
}

std::shared_ptr<SymbolTable>
VctLexerHandler::GetSymbolTable() const
{
    return vct_flex_lexer_->GetSymbolTable();
}
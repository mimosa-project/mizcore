#include <memory>

#undef yyFlexLexer
#define yyFlexLexer yyVctFlexLexer
#include <FlexLexer.h>

#include "vct_file_handler.hpp"
#include "vct_flex_lexer.hpp"

using emcore::mizar::SymbolTable;
using emcore::mizar::VctFileHandler;
using emcore::mizar::VctFlexLexer;

VctFileHandler::VctFileHandler(std::istream* in)
  : vct_flex_lexer_(std::make_shared<VctFlexLexer>(in))
{}

int
VctFileHandler::yylex()
{
    return vct_flex_lexer_->yylex();
}

std::shared_ptr<SymbolTable>
VctFileHandler::GetSymbolTable() const
{
    return vct_flex_lexer_->GetSymbolTable();
}
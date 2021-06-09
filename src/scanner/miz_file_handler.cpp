#include <memory>

#undef yyFlexLexer
#define yyFlexLexer yyMizFlexLexer
#include <FlexLexer.h>

#include "miz_file_handler.hpp"
#include "miz_flex_lexer.hpp"

using mizcore::MizFileHandler;
using mizcore::MizFlexLexer;
using mizcore::SymbolTable;
using mizcore::TokenTable;

MizFileHandler::MizFileHandler(std::istream* in,
                               const std::shared_ptr<SymbolTable>& symbol_table)
  : miz_flex_lexer_(std::make_shared<MizFlexLexer>(in, symbol_table))
{}

int
MizFileHandler::yylex()
{
    return miz_flex_lexer_->yylex();
}

std::shared_ptr<TokenTable>
MizFileHandler::GetTokenTable() const
{
    return miz_flex_lexer_->GetTokenTable();
}

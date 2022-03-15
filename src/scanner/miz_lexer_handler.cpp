#include <memory>

#undef yyFlexLexer
#define yyFlexLexer yyMizFlexLexer
#include <FlexLexer.h>

#include "miz_flex_lexer.hpp"
#include "miz_lexer_handler.hpp"
#include "symbol_table.hpp"

using mizcore::MizFlexLexer;
using mizcore::MizLexerHandler;
using mizcore::SymbolTable;
using mizcore::TokenTable;

MizLexerHandler::MizLexerHandler(
  std::istream* in,
  const std::shared_ptr<SymbolTable>& symbol_table)
  : miz_flex_lexer_(std::make_shared<MizFlexLexer>(in, symbol_table))
{}

int
MizLexerHandler::yylex()
{
    if (is_partial_mode_) {
        // Although SymbolTable::BuildQueryMap() is invoked by "begin" keyword
        // in the tokenizer, the partial tokenizing text may not include "begin"
        // keyword.
        auto symbol_table = miz_flex_lexer_->GetSymbolTable();
        symbol_table->BuildQueryMap();
    }
    return miz_flex_lexer_->yylex();
}

std::shared_ptr<TokenTable>
MizLexerHandler::GetTokenTable() const
{
    return miz_flex_lexer_->GetTokenTable();
}

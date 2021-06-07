%{
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "vct_flex_lexer.hpp"

#undef  YY_DECL
#define YY_DECL int emcore::VctFlexLexer::yylex()
#define YY_NO_UNISTD_H

using std::vector;
using std::string;
using emcore::Symbol;
using emcore::VctFlexLexer;

VctFlexLexer::VctFlexLexer(std::istream *in)
    : yyVctFlexLexer(in),
      symbol_table_(std::make_shared<SymbolTable>())
{
    symbol_table_->Initialize();
}

%}

%option debug
%option nodefault
%option yyclass="emcore::VctFlexLexer"
%option noyywrap
%option c++
%option prefix="yyVct"

FILENAME    [[:upper:]][[:upper:][:digit:]_]+
TYPE        [GKLMORUV]
NUMBER      [[:digit:]]+
SYMBOL      [[:graph:]]+
SPACES      [[:space:]]+

%%
    /* File name */
"#"{FILENAME} {
    current_mizfile_ = yytext + 1;
}

    /* Number of types -> ignore */
"G"{NUMBER}" K"{NUMBER}" L"{NUMBER}" M"{NUMBER}" O"{NUMBER}" R"{NUMBER}" U"{NUMBER}" V"{NUMBER} {
}

    /* Functor with priority */
"O"{SYMBOL}" "{NUMBER} {
    SYMBOL_TYPE type = SYMBOL_TYPE(yytext[0]);

    std::istringstream iss(yytext + 1);
    std::string name;
    int priority;
    iss >> name >> priority;

    symbol_table_->AddSymbol(current_mizfile_, name, type, (uint8_t)priority);
}

    /* Definition of two symbols */
{TYPE}{SYMBOL}" "{SYMBOL} {
    SYMBOL_TYPE type = SYMBOL_TYPE(yytext[0]);

    std::istringstream iss(yytext + 1);
    std::string name0;
    std::string name1;
    iss >> name0 >> name1;

    Symbol* s0 = symbol_table_->AddSymbol(current_mizfile_, name0, type);
    Symbol* s1 = symbol_table_->AddSymbol(current_mizfile_, name1, type);

    symbol_table_->AddSynonym(s0, s1);
}

    /* Ordinary symbol definition */
{TYPE}{SYMBOL} {
    symbol_table_->AddSymbol(current_mizfile_, yytext + 1, SYMBOL_TYPE(yytext[0]));
}

    /* spaces -> ignore */
{SPACES} { }

<<EOF>> {
    current_mizfile_.clear();
    return 0;
}

    /* others -> ignore */
. {}

%%


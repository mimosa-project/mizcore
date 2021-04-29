%{
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "vct_file_handler.hpp"

#undef  YY_DECL
#define YY_DECL int emcore::mizar::VctFileHandler::yylex()
#define YY_NO_UNISTD_H

using std::vector;
using std::string;
using emcore::mizar::Symbol;
using emcore::mizar::VctFileHandler;

VctFileHandler::VctFileHandler(std::istream *in)
    : yyVctFlexLexer(in),
      symbol_table_(std::make_shared<SymbolTable>())
{
}

%}

%option debug
%option nodefault
%option yyclass="emcore::mizar::VctFileHandler"
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

    Symbol* symbol = new Symbol(std::move(name), type, (uint8_t)priority);
    symbol_table_->AddSymbol(current_mizfile_, symbol);
}

    /* Definition of two symbols */
{TYPE}{SYMBOL}" "{SYMBOL} {
    SYMBOL_TYPE type = SYMBOL_TYPE(yytext[0]);

    std::istringstream iss(yytext + 1);
    std::string name0;
    std::string name1;
    iss >> name0 >> name1;

    Symbol* s0 = new Symbol(std::move(name0), type);
    Symbol* s1 = new Symbol(std::move(name1), type);

    symbol_table_->AddSymbol(current_mizfile_, s0);
    symbol_table_->AddSymbol(current_mizfile_, s1);

    symbol_table_->AddSynonym(s0, s1);
}

    /* Ordinary symbol definition */
{TYPE}{SYMBOL} {
    Symbol* symbol = new Symbol(yytext + 1, SYMBOL_TYPE(yytext[0]));
    symbol_table_->AddSymbol(current_mizfile_, symbol);
}

    /* spaces -> ignore */
{SPACES} { }

<<EOF>> {
    current_mizfile_.clear();
    return 0;
}
%%


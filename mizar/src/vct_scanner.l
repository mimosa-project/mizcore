%{
#include "vct_file_handler.hpp"
#include <sstream>
#include <iostream>
#include <stdio.h>

#undef  YY_DECL
#define YY_DECL int emcore::mizar::VctFileHandler::yylex()
#define YY_NO_UNISTD_H

using std::vector;
using std::string;
using emcore::mizar::Symbol;
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
    file2symbols_[current_mizfile_] = vector<Symbol>();
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

    Symbol symbol(std::move(name), type, (uint8_t)priority);
    file2symbols_[current_mizfile_].push_back(std::move(symbol));
}

    /* Definition of two symbols */
{TYPE}{SYMBOL}" "{SYMBOL} {
    SYMBOL_TYPE type = SYMBOL_TYPE(yytext[0]);

    std::istringstream iss(yytext + 1);
    std::string name0;
    std::string name1;
    iss >> name0 >> name1;

    synonyms_.push_back(std::make_pair(name0, name1));

    Symbol s0(std::move(name0), type);
    Symbol s1(std::move(name1), type);

    file2symbols_[current_mizfile_].push_back(std::move(s0));
    file2symbols_[current_mizfile_].push_back(std::move(s1));
}

    /* Ordinary symbol definition */
{TYPE}{SYMBOL} {
    Symbol s(yytext + 1, SYMBOL_TYPE(yytext[0]));
    file2symbols_[current_mizfile_].push_back(std::move(s));
}

    /* spaces -> ignore */
{SPACES} { }

<<EOF>> {
    current_mizfile_.clear();
    return 0;
}
%%


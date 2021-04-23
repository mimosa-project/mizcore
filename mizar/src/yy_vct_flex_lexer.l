%{
#include <sstream>
#include <iostream>
#include <stdio.h>
#include "symbol.hpp"
#include "vct_file_handler.hpp"

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
    file2symbols_[current_mizfile_] = vector<SharedPtr>();
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

    SharedPtr s(new Symbol(std::move(name), type, (uint8_t)priority));
    file2symbols_[current_mizfile_].push_back(s);
}

    /* Definition of two symbols */
{TYPE}{SYMBOL}" "{SYMBOL} {
    SYMBOL_TYPE type = SYMBOL_TYPE(yytext[0]);

    std::istringstream iss(yytext + 1);
    std::string name0;
    std::string name1;
    iss >> name0 >> name1;

    SharedPtr s0(new Symbol(std::move(name0), type));
    SharedPtr s1(new Symbol(std::move(name1), type));

    file2symbols_[current_mizfile_].push_back(s0);
    file2symbols_[current_mizfile_].push_back(s1);

    synonyms_.push_back(std::make_pair(s0, s1));
}

    /* Ordinary symbol definition */
{TYPE}{SYMBOL} {
    SharedPtr s(new Symbol(yytext + 1, SYMBOL_TYPE(yytext[0])));
    file2symbols_[current_mizfile_].push_back(s);
}

    /* spaces -> ignore */
{SPACES} { }

<<EOF>> {
    current_mizfile_.clear();
    return 0;
}
%%


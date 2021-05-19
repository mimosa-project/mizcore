%{
#include <sstream>
#include <iostream>
#include <cassert>
#include <memory>

#include "symbol_table.hpp"
#include "token.hpp"
#include "token_array.hpp"
#include "miz_flex_lexer.hpp"

#undef  YY_DECL
#define YY_DECL int emcore::mizar::MizFlexLexer::yylex()
#define YY_NO_UNISTD_H

%}

%option debug
%option nodefault
%option yyclass="emcore::mizar::MizFlexLexer"
%option noyywrap
%option c++
%option prefix="yyMiz"

SYMBOL      [[:graph:]]+
NUMERAL     "0"|[1-9][0-9]*
FILENAME    [[:upper:]][[:upper:][:digit:]_]{4,7}
IDENTIFIER  [[:alpha:][:digit:]_'z]+
RETURN      "\r\n"|"\r"|"\n"
SPACES      [[:blank:]]+

%%

":::"[^\n\r]*    {
    ScanComment(TOKEN_TYPE::TRIPLE_COMMENT);
}

"::"[^\n\r]*    {
    ScanComment(TOKEN_TYPE::DOUBLE_COMMENT);
}

{FILENAME} {
    if (0 == ScanFileName()) {
        ScanIdentifier();
    }
}

{SYMBOL} {
    size_t scanned_length = ScanSymbol();
    if (scanned_length == 0) {
        REJECT;
    } else {
        yyless(scanned_length);
    }
}

{NUMERAL}       ScanNumeral();

aggregate       ScanKeyword(TOKEN_TYPE::AGGREGATE);
all             ScanKeyword(TOKEN_TYPE::ALL);
and             ScanKeyword(TOKEN_TYPE::AND);
antonym         ScanKeyword(TOKEN_TYPE::ANTONYM);
are             ScanKeyword(TOKEN_TYPE::ARE);
as              ScanKeyword(TOKEN_TYPE::AS);
associativity   ScanKeyword(TOKEN_TYPE::ASSOCIATIVITY);
assume          ScanKeyword(TOKEN_TYPE::ASSUME);
asymmetry       ScanKeyword(TOKEN_TYPE::ASYMMETRY);
attr            ScanKeyword(TOKEN_TYPE::ATTR);
be              ScanKeyword(TOKEN_TYPE::BE);
begin           ScanKeyword(TOKEN_TYPE::BEGIN_);
being           ScanKeyword(TOKEN_TYPE::BEING);
by              ScanKeyword(TOKEN_TYPE::BY);
canceled        ScanKeyword(TOKEN_TYPE::CANCELED);
case            ScanKeyword(TOKEN_TYPE::CASE);
cases           ScanKeyword(TOKEN_TYPE::CASES);
cluster         ScanKeyword(TOKEN_TYPE::CLUSTER);
coherence       ScanKeyword(TOKEN_TYPE::COHERENCE);
commutativity   ScanKeyword(TOKEN_TYPE::COMMUTATIVITY);
compatibility   ScanKeyword(TOKEN_TYPE::COMPATIBILITY);
connectedness   ScanKeyword(TOKEN_TYPE::CONNECTEDNESS);
consider        ScanKeyword(TOKEN_TYPE::CONSIDER);
consistency     ScanKeyword(TOKEN_TYPE::CONSISTENCY);
constructors    ScanKeyword(TOKEN_TYPE::CONSTRUCTORS);
contradiction   ScanKeyword(TOKEN_TYPE::CONTRADICTION);
correctness     ScanKeyword(TOKEN_TYPE::CORRECTNESS);
def             ScanKeyword(TOKEN_TYPE::DEF);
deffunc         ScanKeyword(TOKEN_TYPE::DEFFUNC);
define          ScanKeyword(TOKEN_TYPE::DEFINE);
definition      ScanKeyword(TOKEN_TYPE::DEFINITION);
definitions     ScanKeyword(TOKEN_TYPE::DEFINITIONS);
defpred         ScanKeyword(TOKEN_TYPE::DEFPRED);
do              ScanKeyword(TOKEN_TYPE::DO);
does            ScanKeyword(TOKEN_TYPE::DOES);
end             ScanKeyword(TOKEN_TYPE::END);
environ         ScanKeyword(TOKEN_TYPE::ENVIRON);
equals          ScanKeyword(TOKEN_TYPE::EQUALS);
ex              ScanKeyword(TOKEN_TYPE::EX);
exactly         ScanKeyword(TOKEN_TYPE::EXACTLY);
existence       ScanKeyword(TOKEN_TYPE::EXISTENCE);
for             ScanKeyword(TOKEN_TYPE::FOR);
from            ScanKeyword(TOKEN_TYPE::FROM);
func            ScanKeyword(TOKEN_TYPE::FUNC);
given           ScanKeyword(TOKEN_TYPE::GIVEN);
hence           ScanKeyword(TOKEN_TYPE::HENCE);
hereby          ScanKeyword(TOKEN_TYPE::HEREBY);
holds           ScanKeyword(TOKEN_TYPE::HOLDS);
idempotence     ScanKeyword(TOKEN_TYPE::IDEMPOTENCE);
identify        ScanKeyword(TOKEN_TYPE::IDENTIFY);
if              ScanKeyword(TOKEN_TYPE::IF);
iff             ScanKeyword(TOKEN_TYPE::IFF);
implies         ScanKeyword(TOKEN_TYPE::IMPLIES);
involutiveness  ScanKeyword(TOKEN_TYPE::INVOLUTIVENESS);
irreflexivity   ScanKeyword(TOKEN_TYPE::IRREFLEXIVITY);
is              ScanKeyword(TOKEN_TYPE::IS);
it              ScanKeyword(TOKEN_TYPE::IT);
let             ScanKeyword(TOKEN_TYPE::LET);
means           ScanKeyword(TOKEN_TYPE::MEANS);
mode            ScanKeyword(TOKEN_TYPE::MODE);
non             ScanKeyword(TOKEN_TYPE::NON);
not             ScanKeyword(TOKEN_TYPE::NOT);
notation        ScanKeyword(TOKEN_TYPE::NOTATION);
notations       ScanKeyword(TOKEN_TYPE::NOTATIONS);
now             ScanKeyword(TOKEN_TYPE::NOW);
of              ScanKeyword(TOKEN_TYPE::OF);
or              ScanKeyword(TOKEN_TYPE::OR);
otherwise       ScanKeyword(TOKEN_TYPE::OTHERWISE);
over            ScanKeyword(TOKEN_TYPE::OVER);
per             ScanKeyword(TOKEN_TYPE::PER);
pred            ScanKeyword(TOKEN_TYPE::PRED);
prefix          ScanKeyword(TOKEN_TYPE::PREFIX);
projectivity    ScanKeyword(TOKEN_TYPE::PROJECTIVITY);
proof           ScanKeyword(TOKEN_TYPE::PROOF);
provided        ScanKeyword(TOKEN_TYPE::PROVIDED);
qua             ScanKeyword(TOKEN_TYPE::QUA);
reconsider      ScanKeyword(TOKEN_TYPE::RECONSIDER);
reduce          ScanKeyword(TOKEN_TYPE::REDUCE);
reducibility    ScanKeyword(TOKEN_TYPE::REDUCIBILITY);
redefine        ScanKeyword(TOKEN_TYPE::REDEFINE);
reflexivity     ScanKeyword(TOKEN_TYPE::REFLEXIVITY);
registration    ScanKeyword(TOKEN_TYPE::REGISTRATION);
registrations   ScanKeyword(TOKEN_TYPE::REGISTRATIONS);
requirements    ScanKeyword(TOKEN_TYPE::REQUIREMENTS);
reserve         ScanKeyword(TOKEN_TYPE::RESERVE);
sch             ScanKeyword(TOKEN_TYPE::SCH);
scheme          ScanKeyword(TOKEN_TYPE::SCHEME);
schemes         ScanKeyword(TOKEN_TYPE::SCHEMES);
section         ScanKeyword(TOKEN_TYPE::SECTION);
selector        ScanKeyword(TOKEN_TYPE::SELECTOR);
set             ScanKeyword(TOKEN_TYPE::SET);
sethood         ScanKeyword(TOKEN_TYPE::SETHOOD);
st              ScanKeyword(TOKEN_TYPE::ST);
struct          ScanKeyword(TOKEN_TYPE::STRUCT);
such            ScanKeyword(TOKEN_TYPE::SUCH);
suppose         ScanKeyword(TOKEN_TYPE::SUPPOSE);
symmetry        ScanKeyword(TOKEN_TYPE::SYMMETRY);
synonym         ScanKeyword(TOKEN_TYPE::SYNONYM);
take            ScanKeyword(TOKEN_TYPE::TAKE);
that            ScanKeyword(TOKEN_TYPE::THAT);
the             ScanKeyword(TOKEN_TYPE::THE);
then            ScanKeyword(TOKEN_TYPE::THEN);
theorem         ScanKeyword(TOKEN_TYPE::THEOREM);
theorems        ScanKeyword(TOKEN_TYPE::THEOREMS);
thesis          ScanKeyword(TOKEN_TYPE::THESIS);
thus            ScanKeyword(TOKEN_TYPE::THUS);
to              ScanKeyword(TOKEN_TYPE::TO);
transitivity    ScanKeyword(TOKEN_TYPE::TRANSITIVITY);
uniqueness      ScanKeyword(TOKEN_TYPE::UNIQUENESS);
vocabularies    ScanKeyword(TOKEN_TYPE::VOCABULARIES);
when            ScanKeyword(TOKEN_TYPE::WHEN);
where           ScanKeyword(TOKEN_TYPE::WHERE);
with            ScanKeyword(TOKEN_TYPE::WITH);
wrt             ScanKeyword(TOKEN_TYPE::WRT);

{IDENTIFIER}    ScanIdentifier();
{RETURN}        {++line_number_; column_number_ = 1;}
{SPACES}        {column_number_ += yyleng;}
<<EOF>>         return 0;
.               ScanUnknown();

%%

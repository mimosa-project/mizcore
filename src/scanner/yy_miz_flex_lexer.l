%{
#include <sstream>
#include <iostream>
#include <cassert>
#include <memory>

#include "symbol_table.hpp"
#include "ast_token.hpp"
#include "token_table.hpp"
#include "miz_flex_lexer.hpp"

#undef  YY_DECL
#define YY_DECL int mizcore::MizFlexLexer::yylex()
#define YY_NO_UNISTD_H

%}

%option debug
%option nodefault
%option yyclass="mizcore::MizFlexLexer"
%option noyywrap
%option c++
%option prefix="yyMiz"

SYMBOL      [[:graph:]]+
NUMERAL     "0"|[1-9][0-9]*
FILENAME    [[:upper:]][[:upper:][:digit:]_]{3,7}
IDENTIFIER  [[:alpha:][:digit:]_']+
RETURN      "\r\n"|"\r"|"\n"
SPACES      [[:blank:]]+
NON_SPACES  [^[:space:]]+

%%

":::"[^\n\r]*    {
    ScanComment(COMMENT_TYPE::TRIPLE);
}

"::"[^\n\r]*    {
    ScanComment(COMMENT_TYPE::DOUBLE);
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

aggregate       ScanKeyword(KEYWORD_TYPE::AGGREGATE);
all             ScanKeyword(KEYWORD_TYPE::ALL);
and             ScanKeyword(KEYWORD_TYPE::AND);
antonym         ScanKeyword(KEYWORD_TYPE::ANTONYM);
are             ScanKeyword(KEYWORD_TYPE::ARE);
as              ScanKeyword(KEYWORD_TYPE::AS);
associativity   ScanKeyword(KEYWORD_TYPE::ASSOCIATIVITY);
assume          ScanKeyword(KEYWORD_TYPE::ASSUME);
asymmetry       ScanKeyword(KEYWORD_TYPE::ASYMMETRY);
attr            ScanKeyword(KEYWORD_TYPE::ATTR);
be              ScanKeyword(KEYWORD_TYPE::BE);
begin           ScanKeyword(KEYWORD_TYPE::BEGIN_);
being           ScanKeyword(KEYWORD_TYPE::BEING);
by              ScanKeyword(KEYWORD_TYPE::BY);
canceled        ScanKeyword(KEYWORD_TYPE::CANCELED);
case            ScanKeyword(KEYWORD_TYPE::CASE);
cases           ScanKeyword(KEYWORD_TYPE::CASES);
cluster         ScanKeyword(KEYWORD_TYPE::CLUSTER);
coherence       ScanKeyword(KEYWORD_TYPE::COHERENCE);
commutativity   ScanKeyword(KEYWORD_TYPE::COMMUTATIVITY);
compatibility   ScanKeyword(KEYWORD_TYPE::COMPATIBILITY);
connectedness   ScanKeyword(KEYWORD_TYPE::CONNECTEDNESS);
consider        ScanKeyword(KEYWORD_TYPE::CONSIDER);
consistency     ScanKeyword(KEYWORD_TYPE::CONSISTENCY);
constructors    ScanKeyword(KEYWORD_TYPE::CONSTRUCTORS);
contradiction   ScanKeyword(KEYWORD_TYPE::CONTRADICTION);
correctness     ScanKeyword(KEYWORD_TYPE::CORRECTNESS);
def             ScanKeyword(KEYWORD_TYPE::DEF);
deffunc         ScanKeyword(KEYWORD_TYPE::DEFFUNC);
define          ScanKeyword(KEYWORD_TYPE::DEFINE);
definition      ScanKeyword(KEYWORD_TYPE::DEFINITION);
definitions     ScanKeyword(KEYWORD_TYPE::DEFINITIONS);
defpred         ScanKeyword(KEYWORD_TYPE::DEFPRED);
do              ScanKeyword(KEYWORD_TYPE::DO);
does            ScanKeyword(KEYWORD_TYPE::DOES);
end             ScanKeyword(KEYWORD_TYPE::END);
environ         ScanKeyword(KEYWORD_TYPE::ENVIRON);
equalities      ScanKeyword(KEYWORD_TYPE::EQUALITIES);
equals          ScanKeyword(KEYWORD_TYPE::EQUALS);
ex              ScanKeyword(KEYWORD_TYPE::EX);
exactly         ScanKeyword(KEYWORD_TYPE::EXACTLY);
existence       ScanKeyword(KEYWORD_TYPE::EXISTENCE);
expansions      ScanKeyword(KEYWORD_TYPE::EXPANSIONS);
for             ScanKeyword(KEYWORD_TYPE::FOR);
from            ScanKeyword(KEYWORD_TYPE::FROM);
func            ScanKeyword(KEYWORD_TYPE::FUNC);
given           ScanKeyword(KEYWORD_TYPE::GIVEN);
hence           ScanKeyword(KEYWORD_TYPE::HENCE);
hereby          ScanKeyword(KEYWORD_TYPE::HEREBY);
holds           ScanKeyword(KEYWORD_TYPE::HOLDS);
idempotence     ScanKeyword(KEYWORD_TYPE::IDEMPOTENCE);
identify        ScanKeyword(KEYWORD_TYPE::IDENTIFY);
if              ScanKeyword(KEYWORD_TYPE::IF);
iff             ScanKeyword(KEYWORD_TYPE::IFF);
implies         ScanKeyword(KEYWORD_TYPE::IMPLIES);
involutiveness  ScanKeyword(KEYWORD_TYPE::INVOLUTIVENESS);
irreflexivity   ScanKeyword(KEYWORD_TYPE::IRREFLEXIVITY);
is              ScanKeyword(KEYWORD_TYPE::IS);
it              ScanKeyword(KEYWORD_TYPE::IT);
let             ScanKeyword(KEYWORD_TYPE::LET);
means           ScanKeyword(KEYWORD_TYPE::MEANS);
mode            ScanKeyword(KEYWORD_TYPE::MODE);
non             ScanKeyword(KEYWORD_TYPE::NON);
not             ScanKeyword(KEYWORD_TYPE::NOT);
notation        ScanKeyword(KEYWORD_TYPE::NOTATION);
notations       ScanKeyword(KEYWORD_TYPE::NOTATIONS);
now             ScanKeyword(KEYWORD_TYPE::NOW);
of              ScanKeyword(KEYWORD_TYPE::OF);
or              ScanKeyword(KEYWORD_TYPE::OR);
otherwise       ScanKeyword(KEYWORD_TYPE::OTHERWISE);
over            ScanKeyword(KEYWORD_TYPE::OVER);
per             ScanKeyword(KEYWORD_TYPE::PER);
pred            ScanKeyword(KEYWORD_TYPE::PRED);
prefix          ScanKeyword(KEYWORD_TYPE::PREFIX);
projectivity    ScanKeyword(KEYWORD_TYPE::PROJECTIVITY);
proof           ScanKeyword(KEYWORD_TYPE::PROOF);
provided        ScanKeyword(KEYWORD_TYPE::PROVIDED);
qua             ScanKeyword(KEYWORD_TYPE::QUA);
reconsider      ScanKeyword(KEYWORD_TYPE::RECONSIDER);
reduce          ScanKeyword(KEYWORD_TYPE::REDUCE);
reducibility    ScanKeyword(KEYWORD_TYPE::REDUCIBILITY);
redefine        ScanKeyword(KEYWORD_TYPE::REDEFINE);
reflexivity     ScanKeyword(KEYWORD_TYPE::REFLEXIVITY);
registration    ScanKeyword(KEYWORD_TYPE::REGISTRATION);
registrations   ScanKeyword(KEYWORD_TYPE::REGISTRATIONS);
requirements    ScanKeyword(KEYWORD_TYPE::REQUIREMENTS);
reserve         ScanKeyword(KEYWORD_TYPE::RESERVE);
sch             ScanKeyword(KEYWORD_TYPE::SCH);
scheme          ScanKeyword(KEYWORD_TYPE::SCHEME);
schemes         ScanKeyword(KEYWORD_TYPE::SCHEMES);
section         ScanKeyword(KEYWORD_TYPE::SECTION);
selector        ScanKeyword(KEYWORD_TYPE::SELECTOR);
set             ScanKeyword(KEYWORD_TYPE::SET);
sethood         ScanKeyword(KEYWORD_TYPE::SETHOOD);
st              ScanKeyword(KEYWORD_TYPE::ST);
struct          ScanKeyword(KEYWORD_TYPE::STRUCT);
such            ScanKeyword(KEYWORD_TYPE::SUCH);
suppose         ScanKeyword(KEYWORD_TYPE::SUPPOSE);
symmetry        ScanKeyword(KEYWORD_TYPE::SYMMETRY);
synonym         ScanKeyword(KEYWORD_TYPE::SYNONYM);
take            ScanKeyword(KEYWORD_TYPE::TAKE);
that            ScanKeyword(KEYWORD_TYPE::THAT);
the             ScanKeyword(KEYWORD_TYPE::THE);
then            ScanKeyword(KEYWORD_TYPE::THEN);
theorem         ScanKeyword(KEYWORD_TYPE::THEOREM);
theorems        ScanKeyword(KEYWORD_TYPE::THEOREMS);
thesis          ScanKeyword(KEYWORD_TYPE::THESIS);
thus            ScanKeyword(KEYWORD_TYPE::THUS);
to              ScanKeyword(KEYWORD_TYPE::TO);
transitivity    ScanKeyword(KEYWORD_TYPE::TRANSITIVITY);
uniqueness      ScanKeyword(KEYWORD_TYPE::UNIQUENESS);
vocabularies    ScanKeyword(KEYWORD_TYPE::VOCABULARIES);
when            ScanKeyword(KEYWORD_TYPE::WHEN);
where           ScanKeyword(KEYWORD_TYPE::WHERE);
with            ScanKeyword(KEYWORD_TYPE::WITH);
wrt             ScanKeyword(KEYWORD_TYPE::WRT);

{IDENTIFIER}    ScanIdentifier();
{RETURN}        {++line_number_; column_number_ = 1;}
{SPACES}        {column_number_ += yyleng;}
<<EOF>>         return 0;
.               ScanUnknown();

%%

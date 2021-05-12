#include <cassert>
#include <cstring>
#include <ostream>
#include <string>
#include <map>

#include "token.hpp"
#include "symbol.hpp"

using std::string;
using std::map;
using emcore::mizar::KEYWORD_TYPE;
using emcore::mizar::Token;
using emcore::mizar::SymbolToken;
using emcore::mizar::IdentifierToken;
using emcore::mizar::KeywordToken;
using emcore::mizar::NumeralToken;
using emcore::mizar::FileNameToken;
using emcore::mizar::CommentToken;
using emcore::mizar::UnknownToken;

const char* Token::QueryTypeText(TOKEN_TYPE type)
{
    static string type_text[] = {
        "UNKNOWN",
        "SYMBOL",
        "IDENTIFIER",
        "KEYWORD",
        "NUMERAL",
        "FILENAME",
        "COMMENT",
    };

    return type_text[size_t(type)].c_str();
}

size_t SymbolToken::GetLength() const
{
    return symbol_->GetName().size();
}

const string& SymbolToken::GetText() const
{
    return symbol_->GetName();
}

size_t KeywordToken::GetLength() const
{
    switch (keyword_type_) {
    case KEYWORD_TYPE::UNKNOWN:
        return 0;
    case KEYWORD_TYPE::AS:
    case KEYWORD_TYPE::BE:
    case KEYWORD_TYPE::BY:
    case KEYWORD_TYPE::DO:
    case KEYWORD_TYPE::EX:
    case KEYWORD_TYPE::IF:
    case KEYWORD_TYPE::IS:
    case KEYWORD_TYPE::IT:
    case KEYWORD_TYPE::OF:
    case KEYWORD_TYPE::OR:
    case KEYWORD_TYPE::ST:
    case KEYWORD_TYPE::TO:
        return 2;
    case KEYWORD_TYPE::ALL:
    case KEYWORD_TYPE::AND:
    case KEYWORD_TYPE::ARE:
    case KEYWORD_TYPE::DEF:
    case KEYWORD_TYPE::END:
    case KEYWORD_TYPE::FOR:
    case KEYWORD_TYPE::IFF:
    case KEYWORD_TYPE::LET:
    case KEYWORD_TYPE::NON:
    case KEYWORD_TYPE::NOT:
    case KEYWORD_TYPE::NOW:
    case KEYWORD_TYPE::PER:
    case KEYWORD_TYPE::QUA:
    case KEYWORD_TYPE::SCH:
    case KEYWORD_TYPE::SET:
    case KEYWORD_TYPE::THE:
    case KEYWORD_TYPE::WRT:
        return 3;
    case KEYWORD_TYPE::ATTR:
    case KEYWORD_TYPE::CASE:
    case KEYWORD_TYPE::DOES:
    case KEYWORD_TYPE::FROM:
    case KEYWORD_TYPE::FUNC:
    case KEYWORD_TYPE::MODE:
    case KEYWORD_TYPE::OVER:
    case KEYWORD_TYPE::PRED:
    case KEYWORD_TYPE::SUCH:
    case KEYWORD_TYPE::TAKE:
    case KEYWORD_TYPE::THAT:
    case KEYWORD_TYPE::THEN:
    case KEYWORD_TYPE::THUS:
    case KEYWORD_TYPE::WHEN:
    case KEYWORD_TYPE::WITH:
        return 4;
    case KEYWORD_TYPE::BEGIN_:
    case KEYWORD_TYPE::BEING:
    case KEYWORD_TYPE::CASES:
    case KEYWORD_TYPE::GIVEN:
    case KEYWORD_TYPE::HENCE:
    case KEYWORD_TYPE::HOLDS:
    case KEYWORD_TYPE::MEANS:
    case KEYWORD_TYPE::PROOF:
    case KEYWORD_TYPE::WHERE:
        return 5;
    case KEYWORD_TYPE::ASSUME:
    case KEYWORD_TYPE::DEFINE:
    case KEYWORD_TYPE::EQUALS:
    case KEYWORD_TYPE::HEREBY:
    case KEYWORD_TYPE::PREFIX:
    case KEYWORD_TYPE::REDUCE:
    case KEYWORD_TYPE::SCHEME:
    case KEYWORD_TYPE::STRUCT:
    case KEYWORD_TYPE::THESIS:
        return 6;
    case KEYWORD_TYPE::ANTONYM:
    case KEYWORD_TYPE::CLUSTER:
    case KEYWORD_TYPE::DEFFUNC:
    case KEYWORD_TYPE::DEFPRED:
    case KEYWORD_TYPE::ENVIRON:
    case KEYWORD_TYPE::EXACTLY:
    case KEYWORD_TYPE::IMPLIES:
    case KEYWORD_TYPE::RESERVE:
    case KEYWORD_TYPE::SCHEMES:
    case KEYWORD_TYPE::SECTION:
    case KEYWORD_TYPE::SETHOOD:
    case KEYWORD_TYPE::SUPPOSE:
    case KEYWORD_TYPE::SYNONYM:
    case KEYWORD_TYPE::THEOREM:
        return 7;
    case KEYWORD_TYPE::CANCELED:
    case KEYWORD_TYPE::CONSIDER:
    case KEYWORD_TYPE::IDENTIFY:
    case KEYWORD_TYPE::NOTATION:
    case KEYWORD_TYPE::PROVIDED:
    case KEYWORD_TYPE::REDEFINE:
    case KEYWORD_TYPE::SELECTOR:
    case KEYWORD_TYPE::SYMMETRY:
    case KEYWORD_TYPE::THEOREMS:
        return 8;
    case KEYWORD_TYPE::ACCORDING:
    case KEYWORD_TYPE::AGGREGATE:
    case KEYWORD_TYPE::ASYMMETRY:
    case KEYWORD_TYPE::COHERENCE:
    case KEYWORD_TYPE::EXISTENCE:
    case KEYWORD_TYPE::NOTATIONS:
    case KEYWORD_TYPE::OTHERWISE:
        return 9;
    case KEYWORD_TYPE::DEFINITION:
    case KEYWORD_TYPE::RECONSIDER:
    case KEYWORD_TYPE::UNIQUENESS:
        return 10;
    case KEYWORD_TYPE::CONSISTENCY:
    case KEYWORD_TYPE::CORRECTNESS:
    case KEYWORD_TYPE::DEFINITIONS:
    case KEYWORD_TYPE::IDEMPOTENCE:
    case KEYWORD_TYPE::REFLEXIVITY:
        return 11;
    case KEYWORD_TYPE::CONSTRUCTORS:
    case KEYWORD_TYPE::PROJECTIVITY:
    case KEYWORD_TYPE::REDUCIBILITY:
    case KEYWORD_TYPE::REGISTRATION:
    case KEYWORD_TYPE::REQUIREMENTS:
    case KEYWORD_TYPE::TRANSITIVITY:
    case KEYWORD_TYPE::VOCABULARIES:
        return 12;
    case KEYWORD_TYPE::ASSOCIATIVITY:
    case KEYWORD_TYPE::COMMUTATIVITY:
    case KEYWORD_TYPE::COMPATIBILITY:
    case KEYWORD_TYPE::CONNECTEDNESS:
    case KEYWORD_TYPE::CONTRADICTION:
    case KEYWORD_TYPE::IRREFLEXIVITY:
    case KEYWORD_TYPE::REGISTRATIONS:
        return 13;
    case KEYWORD_TYPE::INVOLUTIVENESS:
        return 14;
    default:
        assert(false);
        return 0;
    }
}

void Token::Dump(std::ostream& os) const {
    os << "pos = (" << line_number_ << ", " << column_number_ << "), "
       << "type = " << QueryTypeText(GetType()) << ", "
       << "length = " << GetLength() << ", "
       << "text = \"" << GetText() << "\"";
}

void SymbolToken::Dump(std::ostream& os) const {
    Token::Dump(os);
    os << ", symbol_type = " << (char)symbol_->GetType()
       << ", priority = " << (int)symbol_->GetPriority();
}

const string& KeywordToken::GetText() const
{
    return QueryKeywordText(keyword_type_);
}

KEYWORD_TYPE KeywordToken::QueryKeywordType(const char* name)
{
    static map<string, KEYWORD_TYPE> keyword_map = 
    {
        {"according", KEYWORD_TYPE::ACCORDING},
        {"aggregate", KEYWORD_TYPE::AGGREGATE},
        {"all", KEYWORD_TYPE::ALL},
        {"and", KEYWORD_TYPE::AND},
        {"antonym", KEYWORD_TYPE::ANTONYM},
        {"are", KEYWORD_TYPE::ARE},
        {"as", KEYWORD_TYPE::AS},
        {"associativity", KEYWORD_TYPE::ASSOCIATIVITY},
        {"assume", KEYWORD_TYPE::ASSUME},
        {"asymmetry", KEYWORD_TYPE::ASYMMETRY},
        {"attr", KEYWORD_TYPE::ATTR},
        {"be", KEYWORD_TYPE::BE},
        {"begin", KEYWORD_TYPE::BEGIN_},
        {"being", KEYWORD_TYPE::BEING},
        {"by", KEYWORD_TYPE::BY},
        {"canceled", KEYWORD_TYPE::CANCELED},
        {"case", KEYWORD_TYPE::CASE},
        {"cases", KEYWORD_TYPE::CASES},
        {"cluster", KEYWORD_TYPE::CLUSTER},
        {"coherence", KEYWORD_TYPE::COHERENCE},
        {"commutativity", KEYWORD_TYPE::COMMUTATIVITY},
        {"compatibility", KEYWORD_TYPE::COMPATIBILITY},
        {"connectedness", KEYWORD_TYPE::CONNECTEDNESS},
        {"consider", KEYWORD_TYPE::CONSIDER},
        {"consistency", KEYWORD_TYPE::CONSISTENCY},
        {"constructors", KEYWORD_TYPE::CONSTRUCTORS},
        {"contradiction", KEYWORD_TYPE::CONTRADICTION},
        {"correctness", KEYWORD_TYPE::CORRECTNESS},
        {"def", KEYWORD_TYPE::DEF},
        {"deffunc", KEYWORD_TYPE::DEFFUNC},
        {"define", KEYWORD_TYPE::DEFINE},
        {"definition", KEYWORD_TYPE::DEFINITION},
        {"definitions", KEYWORD_TYPE::DEFINITIONS},
        {"defpred", KEYWORD_TYPE::DEFPRED},
        {"do", KEYWORD_TYPE::DO},
        {"does", KEYWORD_TYPE::DOES},
        {"end", KEYWORD_TYPE::END},
        {"environ", KEYWORD_TYPE::ENVIRON},
        {"equals", KEYWORD_TYPE::EQUALS},
        {"ex", KEYWORD_TYPE::EX},
        {"exactly", KEYWORD_TYPE::EXACTLY},
        {"existence", KEYWORD_TYPE::EXISTENCE},
        {"for", KEYWORD_TYPE::FOR},
        {"from", KEYWORD_TYPE::FROM},
        {"func", KEYWORD_TYPE::FUNC},
        {"given", KEYWORD_TYPE::GIVEN},
        {"hence", KEYWORD_TYPE::HENCE},
        {"hereby", KEYWORD_TYPE::HEREBY},
        {"holds", KEYWORD_TYPE::HOLDS},
        {"idempotence", KEYWORD_TYPE::IDEMPOTENCE},
        {"identify", KEYWORD_TYPE::IDENTIFY},
        {"if", KEYWORD_TYPE::IF},
        {"iff", KEYWORD_TYPE::IFF},
        {"implies", KEYWORD_TYPE::IMPLIES},
        {"involutiveness", KEYWORD_TYPE::INVOLUTIVENESS},
        {"irreflexivity", KEYWORD_TYPE::IRREFLEXIVITY},
        {"is", KEYWORD_TYPE::IS},
        {"it", KEYWORD_TYPE::IT},
        {"let", KEYWORD_TYPE::LET},
        {"means", KEYWORD_TYPE::MEANS},
        {"mode", KEYWORD_TYPE::MODE},
        {"non", KEYWORD_TYPE::NON},
        {"not", KEYWORD_TYPE::NOT},
        {"notation", KEYWORD_TYPE::NOTATION},
        {"notations", KEYWORD_TYPE::NOTATIONS},
        {"now", KEYWORD_TYPE::NOW},
        {"of", KEYWORD_TYPE::OF},
        {"or", KEYWORD_TYPE::OR},
        {"otherwise", KEYWORD_TYPE::OTHERWISE},
        {"over", KEYWORD_TYPE::OVER},
        {"per", KEYWORD_TYPE::PER},
        {"pred", KEYWORD_TYPE::PRED},
        {"prefix", KEYWORD_TYPE::PREFIX},
        {"projectivity", KEYWORD_TYPE::PROJECTIVITY},
        {"proof", KEYWORD_TYPE::PROOF},
        {"provided", KEYWORD_TYPE::PROVIDED},
        {"qua", KEYWORD_TYPE::QUA},
        {"reconsider", KEYWORD_TYPE::RECONSIDER},
        {"reduce", KEYWORD_TYPE::REDUCE},
        {"reducibility", KEYWORD_TYPE::REDUCIBILITY},
        {"redefine", KEYWORD_TYPE::REDEFINE},
        {"reflexivity", KEYWORD_TYPE::REFLEXIVITY},
        {"registration", KEYWORD_TYPE::REGISTRATION},
        {"registrations", KEYWORD_TYPE::REGISTRATIONS},
        {"requirements", KEYWORD_TYPE::REQUIREMENTS},
        {"reserve", KEYWORD_TYPE::RESERVE},
        {"sch", KEYWORD_TYPE::SCH},
        {"scheme", KEYWORD_TYPE::SCHEME},
        {"schemes", KEYWORD_TYPE::SCHEMES},
        {"section", KEYWORD_TYPE::SECTION},
        {"selector", KEYWORD_TYPE::SELECTOR},
        {"set", KEYWORD_TYPE::SET},
        {"sethood", KEYWORD_TYPE::SETHOOD},
        {"st", KEYWORD_TYPE::ST},
        {"struct", KEYWORD_TYPE::STRUCT},
        {"such", KEYWORD_TYPE::SUCH},
        {"suppose", KEYWORD_TYPE::SUPPOSE},
        {"symmetry", KEYWORD_TYPE::SYMMETRY},
        {"synonym", KEYWORD_TYPE::SYNONYM},
        {"take", KEYWORD_TYPE::TAKE},
        {"that", KEYWORD_TYPE::THAT},
        {"the", KEYWORD_TYPE::THE},
        {"then", KEYWORD_TYPE::THEN},
        {"theorem", KEYWORD_TYPE::THEOREM},
        {"theorems", KEYWORD_TYPE::THEOREMS},
        {"thesis", KEYWORD_TYPE::THESIS},
        {"thus", KEYWORD_TYPE::THUS},
        {"to", KEYWORD_TYPE::TO},
        {"transitivity", KEYWORD_TYPE::TRANSITIVITY},
        {"uniqueness", KEYWORD_TYPE::UNIQUENESS},
        {"vocabularies", KEYWORD_TYPE::VOCABULARIES},
        {"when", KEYWORD_TYPE::WHEN},
        {"where", KEYWORD_TYPE::WHERE},
        {"with", KEYWORD_TYPE::WITH},
        {"wrt", KEYWORD_TYPE::WRT},
    };

    auto it = keyword_map.find(name);
    if (it != keyword_map.end()) {
        return it->second;
    } else {
        return KEYWORD_TYPE::UNKNOWN;
    }
}

const std::string& KeywordToken::QueryKeywordText(KEYWORD_TYPE keyword_type)
{
    static string keyword_texts[] = {
        "", // unknown
        "according",
        "aggregate",
        "all",
        "and",
        "antonym",
        "are",
        "as",
        "associativity",
        "assume",
        "asymmetry",
        "attr",
        "be",
        "begin",
        "being",
        "by",
        "canceled",
        "case",
        "cases",
        "cluster",
        "coherence",
        "commutativity",
        "compatibility",
        "connectedness",
        "consider",
        "consistency",
        "constructors",
        "contradiction",
        "correctness",
        "def",
        "deffunc",
        "define",
        "definition",
        "definitions",
        "defpred",
        "do",
        "does",
        "end",
        "environ",
        "equals",
        "ex",
        "exactly",
        "existence",
        "for",
        "from",
        "func",
        "given",
        "hence",
        "hereby",
        "holds",
        "idempotence",
        "identify",
        "if",
        "iff",
        "implies",
        "involutiveness",
        "irreflexivity",
        "is",
        "it",
        "let",
        "means",
        "mode",
        "non",
        "not",
        "notation",
        "notations",
        "now",
        "of",
        "or",
        "otherwise",
        "over",
        "per",
        "pred",
        "prefix",
        "projectivity",
        "proof",
        "provided",
        "qua",
        "reconsider",
        "reduce",
        "reducibility",
        "redefine",
        "reflexivity",
        "registration",
        "registrations",
        "requirements",
        "reserve",
        "sch",
        "scheme",
        "schemes",
        "section",
        "selector",
        "set",
        "sethood",
        "st",
        "struct",
        "such",
        "suppose",
        "symmetry",
        "synonym",
        "take",
        "that",
        "the",
        "then",
        "theorem",
        "theorems",
        "thesis",
        "thus",
        "to",
        "transitivity",
        "uniqueness",
        "vocabularies",
        "when",
        "where",
        "with",
        "wrt"
    };

    return keyword_texts[(int)keyword_type];
}

#include <array>
#include <cassert>
#include <cstring>
#include <iomanip>
#include <map>
#include <new>
#include <ostream>
#include <string>

#include "symbol.hpp"
#include "token.hpp"

using std::map;
using std::string;

using json = nlohmann::json;

using mizcore::COMMENT_TYPE;
using mizcore::IDENTIFIER_TYPE;
using mizcore::KEYWORD_TYPE;
using mizcore::TOKEN_TYPE;

using mizcore::CommentToken;
using mizcore::IdentifierToken;
using mizcore::KeywordToken;
using mizcore::SymbolToken;
using mizcore::Token;

json
Token::ToJson() const
{
    json j = { { "pos", { line_number_, column_number_ } },
               { "length", GetText().size() },
               { "type", string(QueryTypeText(GetTokenType())) },
               { "text", string(GetText()) } };
    return j;
}

std::string_view
Token::QueryTypeText(TOKEN_TYPE type)
{
    static std::array<string, 6> type2text = { "unknown", "numeral",
                                               "symbol",  "identifier",
                                               "keyword", "comment" };
    return type2text[static_cast<size_t>(type)];
}

std::string_view
SymbolToken::GetText() const
{
    return symbol_->GetText();
}

nlohmann::json
SymbolToken::ToJson() const
{
    json j = Token::ToJson();
    j["symbol_type"] = symbol_->GetTypeString();
    j["priority"] = static_cast<int>(symbol_->GetPriority());
    return j;
}

nlohmann::json
IdentifierToken::ToJson() const
{
    json j = Token::ToJson();
    j["identifier_type"] = QueryIdentifierTypeText(identifier_type_);
    return j;
}

std::string_view
IdentifierToken::QueryIdentifierTypeText(IDENTIFIER_TYPE type)
{
    static std::array<string, 4> type2text = {
        "unknown", "label", "variable", "filename"
    };
    return type2text[static_cast<size_t>(type)];
}

std::string_view
CommentToken::QueryCommentTypeText(COMMENT_TYPE type)
{
    static std::array<string, 3> type2text = { "unknown", "double", "triple" };
    return type2text[static_cast<size_t>(type)];
}

std::string_view
KeywordToken::QueryKeywordText(KEYWORD_TYPE type)
{
    static std::array<string, 118> keyword_text = {
        "unknown",
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
        "equalities",
        "equals",
        "ex",
        "exactly",
        "existence",
        "expansions",
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
        "wrt",
    };

    return keyword_text[static_cast<size_t>(type)].c_str();
}

KEYWORD_TYPE
KeywordToken::QueryKeywordType(std::string_view text)
{
    static map<string, KEYWORD_TYPE> keyword_map = {
        { "according", KEYWORD_TYPE::ACCORDING },
        { "aggregate", KEYWORD_TYPE::AGGREGATE },
        { "all", KEYWORD_TYPE::ALL },
        { "and", KEYWORD_TYPE::AND },
        { "antonym", KEYWORD_TYPE::ANTONYM },
        { "are", KEYWORD_TYPE::ARE },
        { "as", KEYWORD_TYPE::AS },
        { "associativity", KEYWORD_TYPE::ASSOCIATIVITY },
        { "assume", KEYWORD_TYPE::ASSUME },
        { "asymmetry", KEYWORD_TYPE::ASYMMETRY },
        { "attr", KEYWORD_TYPE::ATTR },
        { "be", KEYWORD_TYPE::BE },
        { "begin", KEYWORD_TYPE::BEGIN_ },
        { "being", KEYWORD_TYPE::BEING },
        { "by", KEYWORD_TYPE::BY },
        { "canceled", KEYWORD_TYPE::CANCELED },
        { "case", KEYWORD_TYPE::CASE },
        { "cases", KEYWORD_TYPE::CASES },
        { "cluster", KEYWORD_TYPE::CLUSTER },
        { "coherence", KEYWORD_TYPE::COHERENCE },
        { "commutativity", KEYWORD_TYPE::COMMUTATIVITY },
        { "compatibility", KEYWORD_TYPE::COMPATIBILITY },
        { "connectedness", KEYWORD_TYPE::CONNECTEDNESS },
        { "consider", KEYWORD_TYPE::CONSIDER },
        { "consistency", KEYWORD_TYPE::CONSISTENCY },
        { "constructors", KEYWORD_TYPE::CONSTRUCTORS },
        { "contradiction", KEYWORD_TYPE::CONTRADICTION },
        { "correctness", KEYWORD_TYPE::CORRECTNESS },
        { "def", KEYWORD_TYPE::DEF },
        { "deffunc", KEYWORD_TYPE::DEFFUNC },
        { "define", KEYWORD_TYPE::DEFINE },
        { "definition", KEYWORD_TYPE::DEFINITION },
        { "definitions", KEYWORD_TYPE::DEFINITIONS },
        { "defpred", KEYWORD_TYPE::DEFPRED },
        { "do", KEYWORD_TYPE::DO },
        { "does", KEYWORD_TYPE::DOES },
        { "end", KEYWORD_TYPE::END },
        { "environ", KEYWORD_TYPE::ENVIRON },
        { "equalities", KEYWORD_TYPE::EQUALITIES },
        { "equals", KEYWORD_TYPE::EQUALS },
        { "ex", KEYWORD_TYPE::EX },
        { "exactly", KEYWORD_TYPE::EXACTLY },
        { "existence", KEYWORD_TYPE::EXISTENCE },
        { "expansions", KEYWORD_TYPE::EXPANSIONS },
        { "for", KEYWORD_TYPE::FOR },
        { "from", KEYWORD_TYPE::FROM },
        { "func", KEYWORD_TYPE::FUNC },
        { "given", KEYWORD_TYPE::GIVEN },
        { "hence", KEYWORD_TYPE::HENCE },
        { "hereby", KEYWORD_TYPE::HEREBY },
        { "holds", KEYWORD_TYPE::HOLDS },
        { "idempotence", KEYWORD_TYPE::IDEMPOTENCE },
        { "identify", KEYWORD_TYPE::IDENTIFY },
        { "if", KEYWORD_TYPE::IF },
        { "iff", KEYWORD_TYPE::IFF },
        { "implies", KEYWORD_TYPE::IMPLIES },
        { "involutiveness", KEYWORD_TYPE::INVOLUTIVENESS },
        { "irreflexivity", KEYWORD_TYPE::IRREFLEXIVITY },
        { "is", KEYWORD_TYPE::IS },
        { "it", KEYWORD_TYPE::IT },
        { "let", KEYWORD_TYPE::LET },
        { "means", KEYWORD_TYPE::MEANS },
        { "mode", KEYWORD_TYPE::MODE },
        { "non", KEYWORD_TYPE::NON },
        { "not", KEYWORD_TYPE::NOT },
        { "notation", KEYWORD_TYPE::NOTATION },
        { "notations", KEYWORD_TYPE::NOTATIONS },
        { "now", KEYWORD_TYPE::NOW },
        { "of", KEYWORD_TYPE::OF },
        { "or", KEYWORD_TYPE::OR },
        { "otherwise", KEYWORD_TYPE::OTHERWISE },
        { "over", KEYWORD_TYPE::OVER },
        { "per", KEYWORD_TYPE::PER },
        { "pred", KEYWORD_TYPE::PRED },
        { "prefix", KEYWORD_TYPE::PREFIX },
        { "projectivity", KEYWORD_TYPE::PROJECTIVITY },
        { "proof", KEYWORD_TYPE::PROOF },
        { "provided", KEYWORD_TYPE::PROVIDED },
        { "qua", KEYWORD_TYPE::QUA },
        { "reconsider", KEYWORD_TYPE::RECONSIDER },
        { "reduce", KEYWORD_TYPE::REDUCE },
        { "reducibility", KEYWORD_TYPE::REDUCIBILITY },
        { "redefine", KEYWORD_TYPE::REDEFINE },
        { "reflexivity", KEYWORD_TYPE::REFLEXIVITY },
        { "registration", KEYWORD_TYPE::REGISTRATION },
        { "registrations", KEYWORD_TYPE::REGISTRATIONS },
        { "requirements", KEYWORD_TYPE::REQUIREMENTS },
        { "reserve", KEYWORD_TYPE::RESERVE },
        { "sch", KEYWORD_TYPE::SCH },
        { "scheme", KEYWORD_TYPE::SCHEME },
        { "schemes", KEYWORD_TYPE::SCHEMES },
        { "section", KEYWORD_TYPE::SECTION },
        { "selector", KEYWORD_TYPE::SELECTOR },
        { "set", KEYWORD_TYPE::SET },
        { "sethood", KEYWORD_TYPE::SETHOOD },
        { "st", KEYWORD_TYPE::ST },
        { "struct", KEYWORD_TYPE::STRUCT },
        { "such", KEYWORD_TYPE::SUCH },
        { "suppose", KEYWORD_TYPE::SUPPOSE },
        { "symmetry", KEYWORD_TYPE::SYMMETRY },
        { "synonym", KEYWORD_TYPE::SYNONYM },
        { "take", KEYWORD_TYPE::TAKE },
        { "that", KEYWORD_TYPE::THAT },
        { "the", KEYWORD_TYPE::THE },
        { "then", KEYWORD_TYPE::THEN },
        { "theorem", KEYWORD_TYPE::THEOREM },
        { "theorems", KEYWORD_TYPE::THEOREMS },
        { "thesis", KEYWORD_TYPE::THESIS },
        { "thus", KEYWORD_TYPE::THUS },
        { "to", KEYWORD_TYPE::TO },
        { "transitivity", KEYWORD_TYPE::TRANSITIVITY },
        { "uniqueness", KEYWORD_TYPE::UNIQUENESS },
        { "vocabularies", KEYWORD_TYPE::VOCABULARIES },
        { "when", KEYWORD_TYPE::WHEN },
        { "where", KEYWORD_TYPE::WHERE },
        { "with", KEYWORD_TYPE::WITH },
        { "wrt", KEYWORD_TYPE::WRT },
    };

    auto it = keyword_map.find(string(text));
    if (it != keyword_map.end()) {
        return it->second;
    }
    return KEYWORD_TYPE::UNKNOWN;
}

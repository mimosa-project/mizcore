#include <array>
#include <map>
#include <string>

#include "ast_type.hpp"

using mizcore::BLOCK_TYPE;
using mizcore::COMMENT_TYPE;
using mizcore::ELEMENT_TYPE;
using mizcore::IDENTIFIER_TYPE;
using mizcore::KEYWORD_TYPE;
using mizcore::PATTERN_TYPE;
using mizcore::SPECIAL_SYMBOL_TYPE;
using mizcore::STATEMENT_TYPE;
using mizcore::TOKEN_TYPE;
using std::map;
using std::string;

std::string_view
mizcore::QueryElementTypeText(ELEMENT_TYPE type)
{
    static std::array<string, 3> type2text = {
        "unknown",
        "block",
        "statement",
    };
    return type2text[static_cast<size_t>(type)];
}

BLOCK_TYPE
mizcore::QueryBlockType(KEYWORD_TYPE keyword_type)
{
    switch (keyword_type) {
        case KEYWORD_TYPE::DEFINITION:
            return BLOCK_TYPE::DEFINITION;
        case KEYWORD_TYPE::REGISTRATION:
            return BLOCK_TYPE::REGISTRATION;
        case KEYWORD_TYPE::NOTATION:
            return BLOCK_TYPE::NOTATION;
        case KEYWORD_TYPE::SCHEME:
            return BLOCK_TYPE::SCHEME;
        case KEYWORD_TYPE::CASE:
            return BLOCK_TYPE::CASE;
        case KEYWORD_TYPE::SUPPOSE:
            return BLOCK_TYPE::SUPPOSE;
        case KEYWORD_TYPE::HEREBY:
            return BLOCK_TYPE::HEREBY;
        case KEYWORD_TYPE::NOW:
            return BLOCK_TYPE::NOW;
        case KEYWORD_TYPE::PROOF:
            return BLOCK_TYPE::PROOF;
        default:
            return BLOCK_TYPE::UNKNOWN;
    }
}

std::string_view
mizcore::QueryBlockTypeText(BLOCK_TYPE type)
{
    static std::array<string, 11> type2text = {
        "unknown",  "root",   "definition", "registration",
        "notation", "scheme", "case",       "suppose",
        "hereby",   "now",    "proof",
    };
    return type2text[static_cast<size_t>(type)];
}

STATEMENT_TYPE
mizcore::QueryStatementType(KEYWORD_TYPE keyword_type)
{
    switch (keyword_type) {
        case KEYWORD_TYPE::ANTONYM:
            return STATEMENT_TYPE::ANTONYM;
        case KEYWORD_TYPE::ASSOCIATIVITY:
            return STATEMENT_TYPE::ASSOCIATIVITY;
        case KEYWORD_TYPE::ASSUME:
            return STATEMENT_TYPE::ASSUME;
        case KEYWORD_TYPE::ASYMMETRY:
            return STATEMENT_TYPE::ASYMMETRY;
        case KEYWORD_TYPE::ATTR:
            return STATEMENT_TYPE::ATTR;
        case KEYWORD_TYPE::BEGIN_:
            return STATEMENT_TYPE::BEGIN_;
        case KEYWORD_TYPE::CLUSTER:
            return STATEMENT_TYPE::CLUSTER;
        case KEYWORD_TYPE::COHERENCE:
            return STATEMENT_TYPE::COHERENCE;
        case KEYWORD_TYPE::COMMUTATIVITY:
            return STATEMENT_TYPE::COMMUTATIVITY;
        case KEYWORD_TYPE::CONNECTEDNESS:
            return STATEMENT_TYPE::CONNECTEDNESS;
        case KEYWORD_TYPE::CONSIDER:
            return STATEMENT_TYPE::CONSIDER;
        case KEYWORD_TYPE::CONSISTENCY:
            return STATEMENT_TYPE::CONSISTENCY;
        case KEYWORD_TYPE::CONSTRUCTORS:
            return STATEMENT_TYPE::CONSTRUCTORS;
        case KEYWORD_TYPE::CORRECTNESS:
            return STATEMENT_TYPE::CORRECTNESS;
        case KEYWORD_TYPE::DEFFUNC:
            return STATEMENT_TYPE::DEFFUNC;
        case KEYWORD_TYPE::DEFINITIONS:
            return STATEMENT_TYPE::DEFINITIONS;
        case KEYWORD_TYPE::DEFPRED:
            return STATEMENT_TYPE::DEFPRED;
        case KEYWORD_TYPE::ENVIRON:
            return STATEMENT_TYPE::ENVIRON;
        case KEYWORD_TYPE::EX:
            return STATEMENT_TYPE::EX;
        case KEYWORD_TYPE::EXISTENCE:
            return STATEMENT_TYPE::EXISTENCE;
        case KEYWORD_TYPE::EXPANSIONS:
            return STATEMENT_TYPE::EXPANSIONS;
        case KEYWORD_TYPE::EQUALITIES:
            return STATEMENT_TYPE::EQUALITIES;
        case KEYWORD_TYPE::FOR:
            return STATEMENT_TYPE::FOR;
        case KEYWORD_TYPE::FUNC:
            return STATEMENT_TYPE::FUNC;
        case KEYWORD_TYPE::GIVEN:
            return STATEMENT_TYPE::GIVEN;
        case KEYWORD_TYPE::HENCE:
            return STATEMENT_TYPE::HENCE;
        case KEYWORD_TYPE::IDEMPOTENCE:
            return STATEMENT_TYPE::IDEMPOTENCE;
        case KEYWORD_TYPE::IDENTIFY:
            return STATEMENT_TYPE::IDENTIFY;
        case KEYWORD_TYPE::INVOLUTIVENESS:
            return STATEMENT_TYPE::INVOLUTIVENESS;
        case KEYWORD_TYPE::IRREFLEXIVITY:
            return STATEMENT_TYPE::IRREFLEXIVITY;
        case KEYWORD_TYPE::LET:
            return STATEMENT_TYPE::LET;
        case KEYWORD_TYPE::MODE:
            return STATEMENT_TYPE::MODE;
        case KEYWORD_TYPE::NOTATIONS:
            return STATEMENT_TYPE::NOTATIONS;
        case KEYWORD_TYPE::PER:
            return STATEMENT_TYPE::PER_CASES;
        case KEYWORD_TYPE::PRED:
            return STATEMENT_TYPE::PRED;
        case KEYWORD_TYPE::PROJECTIVITY:
            return STATEMENT_TYPE::PROJECTIVITY;
        case KEYWORD_TYPE::RECONSIDER:
            return STATEMENT_TYPE::RECONSIDER;
        case KEYWORD_TYPE::REDUCE:
            return STATEMENT_TYPE::REDUCE;
        case KEYWORD_TYPE::REDUCIBILITY:
            return STATEMENT_TYPE::REDUCIBILITY;
        case KEYWORD_TYPE::REDEFINE:
            return STATEMENT_TYPE::REDEFINE;
        case KEYWORD_TYPE::REFLEXIVITY:
            return STATEMENT_TYPE::REFLEXIVITY;
        case KEYWORD_TYPE::REGISTRATIONS:
            return STATEMENT_TYPE::REGISTRATIONS;
        case KEYWORD_TYPE::REQUIREMENTS:
            return STATEMENT_TYPE::REQUIREMENTS;
        case KEYWORD_TYPE::RESERVE:
            return STATEMENT_TYPE::RESERVE;
        case KEYWORD_TYPE::SCHEME:
            return STATEMENT_TYPE::SCHEME;
        case KEYWORD_TYPE::SCHEMES:
            return STATEMENT_TYPE::SCHEMES;
        case KEYWORD_TYPE::SECTION:
            return STATEMENT_TYPE::SECTION;
        case KEYWORD_TYPE::SET:
            return STATEMENT_TYPE::SET;
        case KEYWORD_TYPE::SETHOOD:
            return STATEMENT_TYPE::SETHOOD;
        case KEYWORD_TYPE::STRUCT:
            return STATEMENT_TYPE::STRUCT;
        case KEYWORD_TYPE::SYMMETRY:
            return STATEMENT_TYPE::SYMMETRY;
        case KEYWORD_TYPE::SYNONYM:
            return STATEMENT_TYPE::SYNONYM;
        case KEYWORD_TYPE::TAKE:
            return STATEMENT_TYPE::TAKE;
        case KEYWORD_TYPE::THEOREM:
            return STATEMENT_TYPE::THEOREM;
        case KEYWORD_TYPE::THEOREMS:
            return STATEMENT_TYPE::THEOREMS;
        case KEYWORD_TYPE::THUS:
            return STATEMENT_TYPE::THUS;
        case KEYWORD_TYPE::TRANSITIVITY:
            return STATEMENT_TYPE::TRANSITIVITY;
        case KEYWORD_TYPE::UNIQUENESS:
            return STATEMENT_TYPE::UNIQUENESS;
        case KEYWORD_TYPE::VOCABULARIES:
            return STATEMENT_TYPE::VOCABULARIES;
        default:
            return STATEMENT_TYPE::UNKNOWN;
    }
}

std::string_view
mizcore::QueryStatementTypeText(STATEMENT_TYPE type)
{
    static std::array<string, 61> type2text = {
        "unknown",      "antonym",       "associativity",  "assume",
        "asymmetry",    "attr",          "begin",          "cluster",
        "coherence",    "commutativity", "connectedness",  "consider",
        "consistency",  "constructors",  "correctness",    "deffunc",
        "definitions",  "defpred",       "empty",          "environ",
        "ex",           "existence",     "expansions",     "equalities",
        "for",          "func",          "given",          "hence",
        "idempotence",  "identify",      "involutiveness", "irreflexivity",
        "let",          "mode",          "notations",      "per_cases",
        "pred",         "projectivity",  "reconsider",     "reduce",
        "reducibility", "redefine",      "reflexivity",    "registrations",
        "requirements", "reserve",       "scheme",         "schemes",
        "section",      "set",           "sethood",        "struct",
        "symmetry",     "synonym",       "take",           "theorem",
        "theorems",     "thus",          "transitivity",   "uniqueness",
        "vocabularies",
    };
    return type2text[static_cast<size_t>(type)];
}

KEYWORD_TYPE
mizcore::QueryKeywordType(std::string_view text)
{
    static map<string, KEYWORD_TYPE> text2type = {
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

    auto it = text2type.find(string(text));
    if (it != text2type.end()) {
        return it->second;
    }
    return KEYWORD_TYPE::UNKNOWN;
}

std::string_view
mizcore::QueryTokenTypeText(TOKEN_TYPE type)
{
    static std::array<string, 6> type2text = { "unknown", "numeral",
                                               "symbol",  "identifier",
                                               "keyword", "comment" };
    return type2text[static_cast<size_t>(type)];
}

IDENTIFIER_TYPE
mizcore::QueryIdentifierType(std::string_view text)
{
    static map<string, IDENTIFIER_TYPE> text2type = {
        { "label", IDENTIFIER_TYPE::LABEL },
        { "variable", IDENTIFIER_TYPE::VARIABLE },
        { "filename", IDENTIFIER_TYPE::FILENAME },
        { "scheme", IDENTIFIER_TYPE::SCHEME },
        { "predicate", IDENTIFIER_TYPE::PREDICATE },
        { "functor", IDENTIFIER_TYPE::FUNCTOR },
        { "reserved", IDENTIFIER_TYPE::RESERVED },
    };

    auto it = text2type.find(string(text));
    if (it != text2type.end()) {
        return it->second;
    }
    return IDENTIFIER_TYPE::UNKNOWN;
}

std::string_view
mizcore::QueryIdentifierTypeText(IDENTIFIER_TYPE type)
{
    static std::array<string, 8> type2text = { "unknown",  "label",
                                               "variable", "filename",
                                               "scheme",   "predicate",
                                               "functor",  "reserved" };
    return type2text[static_cast<size_t>(type)];
}

std::string_view
mizcore::QueryCommentTypeText(COMMENT_TYPE type)
{
    static std::array<string, 3> type2text = { "unknown", "double", "triple" };
    return type2text[static_cast<size_t>(type)];
}

std::string_view
mizcore::QueryKeywordText(KEYWORD_TYPE type)
{
    static std::array<string, 118> type2text = {
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

    return type2text[static_cast<size_t>(type)];
}

SPECIAL_SYMBOL_TYPE
mizcore::QuerySpecialSymbolType(std::string_view text)
{
    static map<string, SPECIAL_SYMBOL_TYPE> text2type = {
        { ",", SPECIAL_SYMBOL_TYPE::COMMA },
        { ";", SPECIAL_SYMBOL_TYPE::SEMICOLON },
        { ":", SPECIAL_SYMBOL_TYPE::COLON },
        { "(", SPECIAL_SYMBOL_TYPE::LEFT_PARENTHESIS },
        { ")", SPECIAL_SYMBOL_TYPE::RIGHT_PARENTHESIS },
        { "[", SPECIAL_SYMBOL_TYPE::LEFT_BRACKET },
        { "]", SPECIAL_SYMBOL_TYPE::RIGHT_BRACKET },
        { "{", SPECIAL_SYMBOL_TYPE::LEFT_BRACE },
        { "}", SPECIAL_SYMBOL_TYPE::RIGHT_BRACE },
        { "=", SPECIAL_SYMBOL_TYPE::EQUAL },
        { "&", SPECIAL_SYMBOL_TYPE::AND },
        { "->", SPECIAL_SYMBOL_TYPE::ARROW },
        { ".=", SPECIAL_SYMBOL_TYPE::DOT_EQUAL },
        { "...", SPECIAL_SYMBOL_TYPE::THREE_DOTS },
        { "$1", SPECIAL_SYMBOL_TYPE::DOLLAR_1 },
        { "$2", SPECIAL_SYMBOL_TYPE::DOLLAR_2 },
        { "$3", SPECIAL_SYMBOL_TYPE::DOLLAR_3 },
        { "$4", SPECIAL_SYMBOL_TYPE::DOLLAR_4 },
        { "$5", SPECIAL_SYMBOL_TYPE::DOLLAR_5 },
        { "$6", SPECIAL_SYMBOL_TYPE::DOLLAR_6 },
        { "$7", SPECIAL_SYMBOL_TYPE::DOLLAR_7 },
        { "$8", SPECIAL_SYMBOL_TYPE::DOLLAR_8 },
        { "$9", SPECIAL_SYMBOL_TYPE::DOLLAR_9 },
        { "$10", SPECIAL_SYMBOL_TYPE::DOLLAR_10 },
        { "(#", SPECIAL_SYMBOL_TYPE::LEFT_FIELD },
        { "#)", SPECIAL_SYMBOL_TYPE::RIGHT_FIELD },
    };

    auto it = text2type.find(string(text));
    if (it != text2type.end()) {
        return it->second;
    }
    return SPECIAL_SYMBOL_TYPE::UNKNOWN;
}

std::string_view
mizcore::QuerySpecialSymbolText(SPECIAL_SYMBOL_TYPE type)
{
    static std::array<string, 27> type2text = {
        "unknown", ",",  ";",  ":",  "(",  ")",   "[",   "]",  "{",
        "}",       "=",  "&",  "->", ".=", "...", "$1",  "$2", "$3",
        "$4",      "$5", "$6", "$7", "$8", "$9",  "$10", "(#", "#)",
    };
    return type2text[static_cast<size_t>(type)];
}

PATTERN_TYPE
mizcore::QueryPatternType(std::string_view text)
{
    static map<string, PATTERN_TYPE> text2type = {
        { "unknown", PATTERN_TYPE::UNKNOWN },
        { "func", PATTERN_TYPE::FUNCTOR },
        { "bracket_func", PATTERN_TYPE::BRACKET_FUNCTOR },
        { "attr", PATTERN_TYPE::ATTRIBUTE },
        { "pred", PATTERN_TYPE::PREDICATE },
        { "mode", PATTERN_TYPE::MODE },
    };

    auto it = text2type.find(string(text));
    if (it != text2type.end()) {
        return it->second;
    }
    return PATTERN_TYPE::UNKNOWN;
}

std::string_view
mizcore::QueryPatternTypeText(PATTERN_TYPE type)
{
    static std::array<string, 6> type2text = {
        "unknown", "func", "bracket_func", "attr", "pred", "mode",
    };
    return type2text[static_cast<size_t>(type)];
}

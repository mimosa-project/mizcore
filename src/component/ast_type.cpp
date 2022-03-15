#include <array>
#include <string>

#include "ast_type.hpp"

using mizcore::BLOCK_TYPE;
using mizcore::ELEMENT_TYPE;
using mizcore::KEYWORD_TYPE;
using mizcore::STATEMENT_TYPE;
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
            return STATEMENT_TYPE::BEGIN;
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
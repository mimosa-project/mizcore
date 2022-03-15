#pragma once
#include "token_type.hpp"
#include <memory>

namespace mizcore {

enum class ELEMENT_TYPE
{
    UNKNOWN,
    BLOCK,
    STATEMENT,
};

enum class BLOCK_TYPE
{
    UNKNOWN,
    ROOT,
    DEFINITION,
    REGISTRATION,
    NOTATION,
    SCHEME,
    CASE,
    SUPPOSE,
    HEREBY,
    NOW,
    PROOF,
};

enum class STATEMENT_TYPE
{
    UNKNOWN,
    ANTONYM,
    ASSOCIATIVITY,
    ASSUME,
    ASYMMETRY,
    ATTR,
    BEGIN,
    CLUSTER,
    COHERENCE,
    COMMUTATIVITY,
    CONNECTEDNESS,
    CONSIDER,
    CONSISTENCY,
    CONSTRUCTORS,
    CORRECTNESS,
    DEFFUNC,
    DEFINITIONS,
    DEFPRED,
    EMPTY,
    ENVIRON,
    EX,
    EXISTENCE,
    EXPANSIONS,
    EQUALITIES,
    FOR,
    FUNC,
    GIVEN,
    HENCE,
    IDEMPOTENCE,
    IDENTIFY,
    INVOLUTIVENESS,
    IRREFLEXIVITY,
    LET,
    MODE,
    NOTATIONS,
    PER_CASES,
    PRED,
    PROJECTIVITY,
    RECONSIDER,
    REDUCE,
    REDUCIBILITY,
    REDEFINE,
    REFLEXIVITY,
    REGISTRATIONS,
    REQUIREMENTS,
    RESERVE,
    SCHEME,
    SCHEMES,
    SECTION,
    SET,
    SETHOOD,
    STRUCT,
    SYMMETRY,
    SYNONYM,
    TAKE,
    THEOREM,
    THEOREMS,
    THUS,
    TRANSITIVITY,
    UNIQUENESS,
    VOCABULARIES,
};

std::string_view
QueryElementTypeText(ELEMENT_TYPE type);

BLOCK_TYPE
QueryBlockType(KEYWORD_TYPE keyword_type);

std::string_view
QueryBlockTypeText(BLOCK_TYPE type);

STATEMENT_TYPE
QueryStatementType(KEYWORD_TYPE keyword_type);

std::string_view
QueryStatementTypeText(STATEMENT_TYPE type);

} // namespace mizcore

#pragma once

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
    EMPTY,
    ENVIRON,
    SECTION,
    VOCABULARIES,
    NOTATIONS,
    CONSTRUCTORS,
    REGISTRATIONS,
    DEFINITIONS,
    EXPANSIONS,
    EQUALITIES,
    THEOREMS,
    SCHEMES,
    REQUIREMENTS,
    SCHEME,
    THEOREM,
    PER_CASES,
};

} // namespace mizcore

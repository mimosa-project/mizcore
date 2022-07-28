#pragma once
#include "ast_type.hpp"
#include <memory>
#include <string_view>

namespace mizcore {

enum class ELEMENT_TYPE
{
    UNKNOWN,
    BLOCK,
    STATEMENT,
    TOKEN,
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
    BEGIN_,
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

enum class TOKEN_TYPE
{
    UNKNOWN,
    NUMERAL,
    SYMBOL,
    IDENTIFIER,
    KEYWORD,
    COMMENT,
};

enum class IDENTIFIER_TYPE
{
    UNKNOWN,
    LABEL,
    VARIABLE,
    FILENAME,
    SCHEME,
    PREDICATE,
    FUNCTOR,
    RESERVED,
};

enum class COMMENT_TYPE
{
    UNKNOWN,
    DOUBLE,
    TRIPLE,
};

enum class KEYWORD_TYPE
{
    UNKNOWN,
    ACCORDING,
    AGGREGATE,
    ALL,
    AND,
    ANTONYM,
    ARE,
    AS,
    ASSOCIATIVITY,
    ASSUME,
    ASYMMETRY,
    ATTR,
    BE,
    BEGIN_,
    BEING,
    BY,
    CANCELED,
    CASE,
    CASES,
    CLUSTER,
    COHERENCE,
    COMMUTATIVITY,
    COMPATIBILITY,
    CONNECTEDNESS,
    CONSIDER,
    CONSISTENCY,
    CONSTRUCTORS,
    CONTRADICTION,
    CORRECTNESS,
    DEF,
    DEFFUNC,
    DEFINE,
    DEFINITION,
    DEFINITIONS,
    DEFPRED,
    DO,
    DOES,
    END,
    ENVIRON,
    EQUALITIES,
    EQUALS,
    EX,
    EXACTLY,
    EXISTENCE,
    EXPANSIONS,
    FOR,
    FROM,
    FUNC,
    GIVEN,
    HENCE,
    HEREBY,
    HOLDS,
    IDEMPOTENCE,
    IDENTIFY,
    IF,
    IFF,
    IMPLIES,
    INVOLUTIVENESS,
    IRREFLEXIVITY,
    IS,
    IT,
    LET,
    MEANS,
    MODE,
    NON,
    NOT,
    NOTATION,
    NOTATIONS,
    NOW,
    OF,
    OR,
    OTHERWISE,
    OVER,
    PER,
    PRED,
    PREFIX,
    PROJECTIVITY,
    PROOF,
    PROVIDED,
    QUA,
    RECONSIDER,
    REDUCE,
    REDUCIBILITY,
    REDEFINE,
    REFLEXIVITY,
    REGISTRATION,
    REGISTRATIONS,
    REQUIREMENTS,
    RESERVE,
    SCH,
    SCHEME,
    SCHEMES,
    SECTION,
    SELECTOR,
    SET,
    SETHOOD,
    ST,
    STRUCT,
    SUCH,
    SUPPOSE,
    SYMMETRY,
    SYNONYM,
    TAKE,
    THAT,
    THE,
    THEN,
    THEOREM,
    THEOREMS,
    THESIS,
    THUS,
    TO,
    TRANSITIVITY,
    UNIQUENESS,
    VOCABULARIES,
    WHEN,
    WHERE,
    WITH,
    WRT,
};

// R - Predicate,
// O - Functor,
// M - Mode,
// G - Structure,
// U - Selector,
// V - Attribute,
// K - Left Functor Bracket,
// L - Right Functor Bracket.
enum class SYMBOL_TYPE : char
{
    UNKNOWN = '-',
    PREDICATE = 'R',
    FUNCTOR = 'O',
    MODE = 'M',
    STRUCTURE = 'G',
    SELECTOR = 'U',
    ATTRIBUTE = 'V',
    LEFT_FUNCTOR_BRACKET = 'K',
    RIGHT_FUNCTOR_BRACKET = 'L',
    SPECIAL = 'S',
};

enum class SPECIAL_SYMBOL_TYPE
{
    UNKNOWN,
    COMMA,
    SEMICOLON,
    COLON,
    LEFT_PARENTHESIS,
    RIGHT_PARENTHESIS,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    LEFT_BRACE,
    RIGHT_BRACE,
    EQUAL,
    AND,
    ARROW,
    DOT_EQUAL,
    THREE_DOTS,
    DOLLAR_1,
    DOLLAR_2,
    DOLLAR_3,
    DOLLAR_4,
    DOLLAR_5,
    DOLLAR_6,
    DOLLAR_7,
    DOLLAR_8,
    DOLLAR_9,
    DOLLAR_10,
    LEFT_FIELD,
    RIGHT_FIELD,
};

enum class PATTERN_TYPE
{
    UNKNOWN,
    FUNCTOR,
    BRACKET_FUNCTOR,
    ATTRIBUTE,
    PREDICATE,
    MODE
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

KEYWORD_TYPE
QueryKeywordType(std::string_view text);

std::string_view
QueryTokenTypeText(TOKEN_TYPE type);

IDENTIFIER_TYPE
QueryIdentifierType(std::string_view text);

std::string_view
QueryIdentifierTypeText(IDENTIFIER_TYPE type);

std::string_view
QueryCommentTypeText(COMMENT_TYPE type);

std::string_view
QueryKeywordText(KEYWORD_TYPE type);

SPECIAL_SYMBOL_TYPE
QuerySpecialSymbolType(std::string_view text);

std::string_view
QuerySpecialSymbolText(SPECIAL_SYMBOL_TYPE type);

PATTERN_TYPE
QueryPatternType(std::string_view text);

std::string_view
QueryPatternTypeText(PATTERN_TYPE type);

} // namespace mizcore

#pragma once

namespace mizcore {

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

} // namespace mizcore
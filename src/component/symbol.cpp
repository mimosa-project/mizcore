#include "symbol.hpp"

using emcore::Symbol;

std::string_view
Symbol::GetTypeString() const
{
    switch (type_) {
        case SYMBOL_TYPE::UNKNOWN:
            return "-";
        case SYMBOL_TYPE::PREDICATE:
            return "R";
        case SYMBOL_TYPE::FUNCTOR:
            return "O";
        case SYMBOL_TYPE::MODE:
            return "M";
        case SYMBOL_TYPE::STRUCTURE:
            return "G";
        case SYMBOL_TYPE::SELECTOR:
            return "U";
        case SYMBOL_TYPE::ATTRIBUTE:
            return "V";
        case SYMBOL_TYPE::LEFT_FUNCTOR_BRACKET:
            return "K";
        case SYMBOL_TYPE::RIGHT_FUNCTOR_BRACKET:
            return "L";
        case SYMBOL_TYPE::SPECIAL:
            return "S";
        default:
            return "";
    }
}

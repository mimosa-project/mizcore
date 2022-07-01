#include "pattern_element.hpp"

using mizcore::PatternElement;
using json = nlohmann::json;

void
PatternElement::SetFunctorPattern(SymbolToken* symbol_token,
                                  size_t left_arity,
                                  size_t right_arity)
{
    type_ = PATTERN_TYPE::FUNCTOR;
    symbol_tokens_[0] = symbol_token;
    arities_[0] = left_arity;
    arities_[1] = right_arity;
}

void
PatternElement::SetBracketFunctorPattern(SymbolToken* left_symbol_token,
                                         SymbolToken* right_symbol_token,
                                         size_t arity)
{
    type_ = PATTERN_TYPE::BRACKET_FUNCTOR;
    symbol_tokens_[0] = left_symbol_token;
    symbol_tokens_[1] = right_symbol_token;
    arities_[0] = arity;
}

void
PatternElement::SetAttributePattern(SymbolToken* symbol_token, size_t arity)
{
    type_ = PATTERN_TYPE::ATTRIBUTE;
    symbol_tokens_[0] = symbol_token;
    arities_[0] = arity;
}

void
PatternElement::SetModePattern(SymbolToken* symbol_token, size_t arity)
{
    type_ = PATTERN_TYPE::MODE;
    symbol_tokens_[0] = symbol_token;
    arities_[0] = arity;
}

void
PatternElement::SetPredicatePattern(SymbolToken* symbol_token,
                                    size_t left_arity,
                                    size_t right_arity)
{
    type_ = PATTERN_TYPE::PREDICATE;
    symbol_tokens_[0] = symbol_token;
    arities_[0] = left_arity;
    arities_[1] = right_arity;
}

void
PatternElement::ToJson(nlohmann::json& json) const
{
    json = { { "type", QueryPatternTypeText(type_) },
             { "filename", filename_ } };

    json["symbols"].push_back(symbol_tokens_[0]->GetText());
    if (type_ == PATTERN_TYPE::BRACKET_FUNCTOR) {
        json["symbols"].push_back(symbol_tokens_[1]->GetText());
    }
    json["arities"].push_back(arities_[0]);
    if (type_ == PATTERN_TYPE::FUNCTOR || type_ == PATTERN_TYPE::PREDICATE) {
        json["arities"].push_back(arities_[1]);
    }
}

bool
PatternElement::operator<(const PatternElement& other) const
{
    if (type_ < other.type_) {
        return true;
    }
    if (symbol_tokens_[0] < other.symbol_tokens_[0]) {
        return true;
    }
    if (arities_[0] < other.arities_[0]) {
        return true;
    }
    if (arities_[1] < other.arities_[1]) {
        return true;
    }
    return false;
}
#include "token.hpp"
#include "token_array.hpp"

using emcore::mizar::Token;
using emcore::mizar::TokenArray;

void TokenArray::AddToken(Token* token)
{
    tokens_.push_back(std::unique_ptr<Token>(token));
}

std::vector<Token*> TokenArray::CollectTokenArray() const
{
    std::vector<Token*> tokens;
    tokens.reserve(tokens_.size());

    for (const auto& token : tokens_) {
        tokens.push_back(token.get());
    }
    return tokens;
}

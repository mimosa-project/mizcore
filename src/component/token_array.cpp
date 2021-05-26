#include <iomanip>
#include <ostream>

#include "token.hpp"
#include "token_array.hpp"

using emcore::Token;
using emcore::TokenArray;

void
TokenArray::AddToken(Token* token)
{
    tokens_.push_back(std::unique_ptr<Token>(token));
}

void
TokenArray::Dump(std::ostream& os) const
{
    for (size_t i = 0; i < tokens_.size(); ++i) {
        os << std::setw(5) << i << ": ";
        tokens_[i]->Dump(os);
        os << std::endl;
    }
}
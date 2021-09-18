#include <iomanip>
#include <ostream>
#include <sstream>

#include "token.hpp"
#include "token_table.hpp"

using nlohmann::json;

using mizcore::Token;
using mizcore::TokenTable;

void
TokenTable::AddToken(Token* token)
{
    token->SetId(tokens_.size());
    tokens_.emplace_back(token);
}

nlohmann::json
TokenTable::ToJson() const
{
    json j;
    for (const auto& token : tokens_) {
        j.push_back(token->ToJson());
    }
    return j;
}

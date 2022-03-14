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

void
TokenTable::ToJson(nlohmann::json& json) const
{
    for (const auto& token : tokens_) {
        nlohmann::json j;
        token->ToJson(j);
        json.push_back(j);
    }
}

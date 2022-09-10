#include <iomanip>
#include <ostream>
#include <sstream>

#include "ast_token.hpp"
#include "token_table.hpp"

using nlohmann::json;

using mizcore::ASTToken;
using mizcore::TokenTable;

void
TokenTable::AddToken(ASTToken* token)
{
    token->SetId(tokens_.size());
    tokens_.emplace_back(token);
}

void
TokenTable::ReplaceToken(ASTToken* token, size_t i)
{
    token->SetId(i);
    tokens_[i].reset(token);
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

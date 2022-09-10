#include "ast_token.hpp"
#include "symbol.hpp"

using std::string;

using json = nlohmann::json;

using mizcore::SPECIAL_SYMBOL_TYPE;
using mizcore::SYMBOL_TYPE;

using mizcore::ASTToken;
using mizcore::IdentifierToken;
using mizcore::SymbolToken;

void
ASTToken::ToJson(nlohmann::json& json) const
{
    json = { { "id", GetId() },
             { "pos", { line_number_, column_number_ } },
             { "length", GetText().size() },
             { "type", string(QueryTokenTypeText(GetTokenType())) },
             { "text", string(GetText()) } };
}

std::string_view
SymbolToken::GetText() const
{
    return symbol_->GetText();
}

SYMBOL_TYPE
SymbolToken::GetSymbolType() const
{
    return symbol_->GetType();
}

SPECIAL_SYMBOL_TYPE
SymbolToken::GetSpecialSymbolType() const
{
    return symbol_->GetSpecialType();
}

void
SymbolToken::ToJson(nlohmann::json& json) const
{
    ASTToken::ToJson(json);
    json["symbol_type"] = symbol_->GetTypeString();
    json["priority"] = static_cast<int>(symbol_->GetPriority());
}

void
IdentifierToken::ToJson(nlohmann::json& json) const
{
    ASTToken::ToJson(json);
    json["identifier_type"] = QueryIdentifierTypeText(identifier_type_);
    if (ref_token_ != nullptr) {
        json["ref_id"] = ref_token_->GetId();
    }
}

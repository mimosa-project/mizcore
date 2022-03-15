#include "token.hpp"
#include "symbol.hpp"

using std::string;

using json = nlohmann::json;

using mizcore::IdentifierToken;
using mizcore::SymbolToken;
using mizcore::Token;

void
Token::ToJson(nlohmann::json& json) const
{
    json = { { "pos", { line_number_, column_number_ } },
             { "length", GetText().size() },
             { "type", string(QueryTokenTypeText(GetTokenType())) },
             { "text", string(GetText()) } };
}

std::string_view
SymbolToken::GetText() const
{
    return symbol_->GetText();
}

void
SymbolToken::ToJson(nlohmann::json& json) const
{
    Token::ToJson(json);
    json["symbol_type"] = symbol_->GetTypeString();
    json["priority"] = static_cast<int>(symbol_->GetPriority());
}

void
IdentifierToken::ToJson(nlohmann::json& json) const
{
    Token::ToJson(json);
    json["identifier_type"] = QueryIdentifierTypeText(identifier_type_);
}

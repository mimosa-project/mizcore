#include <array>
#include <cassert>
#include <string>

#include "ast_statement.hpp"
#include "token.hpp"

using std::string;

using mizcore::ASTStatement;
using nlohmann::json;

void
ASTStatement::ToJson(nlohmann::json& json) const
{
    ASTComponent::ToJson(json);
    json["statement_type"] = QueryStatementTypeText(statement_type_);
}

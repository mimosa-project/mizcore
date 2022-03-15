#include "ast_element.hpp"
#include "ast_type.hpp"

using mizcore::ASTElement;

void
ASTElement::ToJson(nlohmann::json& json) const
{
    json["type"] = mizcore::QueryElementTypeText(GetElementType());
}

#include "ast_component.hpp"
#include "ast_token.hpp"

using mizcore::ASTComponent;

void
ASTComponent::ToJson(nlohmann::json& json) const
{
    ASTElement::ToJson(json);
    const auto* first_token = GetRangeFirstToken();
    const auto* last_token = GetRangeLastToken();

    if (first_token != nullptr && last_token != nullptr) {
        size_t first_token_id = first_token->GetId();
        size_t last_token_id = last_token->GetId();

        int first_line_no = first_token->GetLineNumber();
        int first_column_no = first_token->GetColumnNumber();
        int last_line_no = last_token->GetLineNumber();
        int last_column_no = last_token->GetColumnNumber();

        if (first_token_id != SIZE_MAX && last_token_id != SIZE_MAX) {
            if (first_token_id == last_token_id) {
                json["token_id"] = first_token_id;
                json["pos"] = { first_line_no, first_column_no };
            } else {
                json["token_id"] = { first_token_id, last_token_id };
                json["pos"] = { { first_line_no, first_column_no },
                                { last_line_no, last_column_no } };
            }
        }
    }
}

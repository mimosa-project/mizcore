#include "ast_component.hpp"
#include "ast_token.hpp"

using mizcore::ASTComponent;

void
ASTComponent::ToJson(nlohmann::json& json) const
{
    ASTElement::ToJson(json);
    const auto* start_token = GetRangeStartToken();
    const auto* end_token = GetRangeEndToken();

    if (start_token != nullptr && end_token != nullptr) {
        size_t start_token_id = start_token->GetId();
        size_t end_token_id = end_token->GetId();

        int start_line_no = start_token->GetLineNumber();
        int start_column_no = start_token->GetColumnNumber();
        int end_line_no = end_token->GetLineNumber();
        int end_column_no = end_token->GetColumnNumber();

        if (start_token_id != SIZE_MAX && end_token_id != SIZE_MAX) {
            if (start_token_id == end_token_id) {
                json["token_id"] = start_token_id;
                json["pos"] = { start_line_no, start_column_no };
            } else {
                json["token_id"] = { start_token_id, end_token_id };
                json["pos"] = { { start_line_no, start_column_no },
                                { end_line_no, end_column_no } };
            }
        }
    }
}

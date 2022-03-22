#include <sstream>

#include "ast_token.hpp"
#include "error_object.hpp"

using mizcore::ErrorObject;

std::string
ErrorObject::GetMessage() const
{
    std::ostringstream oss;
    auto error_level = mizcore::GetErrorLevel(error_type_);
    auto error_code = mizcore::GetErrorCode(error_type_);

    oss << "[" << mizcore::GetErrorLevelText(error_level) << "]";

    if (ast_token_ != nullptr) {
        int line_no = ast_token_->GetLineNumber();
        int column_no = ast_token_->GetColumnNumber();
        oss << " (" << line_no << ", " << column_no << "): ";
    }

    oss << "C" << static_cast<int>(error_code) << ": "
        << mizcore::GetErrorMessage(error_type_);

    return oss.str();
}

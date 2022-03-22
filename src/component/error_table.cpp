#include <algorithm>
#include <string>

#include "ast_token.hpp"
#include "error_object.hpp"
#include "error_table.hpp"
#include "spdlog/spdlog.h"

using mizcore::ASTToken;
using mizcore::ErrorObject;
using mizcore::ErrorTable;

void
ErrorTable::AddError(ErrorObject* error)
{
    errors_.emplace_back(error);
}

void
ErrorTable::LogErrors()
{
    SortErrors();

    for (const auto& error : errors_) {
        spdlog::error(error->GetMessage());
    }
}

void
ErrorTable::SortErrors()
{
    std::stable_sort(errors_.begin(),
                     errors_.end(),
                     [](const std::unique_ptr<ErrorObject>& a,
                        const std::unique_ptr<ErrorObject>& b) -> bool {
                         ASTToken* token_a = a->GetASTToken();
                         ASTToken* token_b = b->GetASTToken();
                         assert(token_a);
                         assert(token_b);
                         return token_a->GetId() < token_b->GetId();
                     });
}

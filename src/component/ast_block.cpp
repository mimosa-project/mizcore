#include <cassert>

#include "ast_block.hpp"
#include "ast_statement.hpp"

using mizcore::ASTBlock;
using mizcore::ASTStatement;

ASTBlock*
ASTBlock::GetChildBlock(size_t i) const
{
    auto* component = GetChildComponent(i);
    assert(component != nullptr);
    return component->GetElementType() == ELEMENT_TYPE::BLOCK
             ? static_cast<ASTBlock*>(component)
             : nullptr;
}

ASTStatement*
ASTBlock::GetChildStatement(size_t i) const
{
    auto* component = GetChildComponent(i);
    assert(component != nullptr);
    return component->GetElementType() == ELEMENT_TYPE::STATEMENT
             ? static_cast<ASTStatement*>(component)
             : nullptr;
}

void
ASTBlock::ToJson(nlohmann::json& json) const
{
    ASTComponent::ToJson(json);
    for (const auto& child_component : child_components_) {
        nlohmann::json child_json;
        child_component->ToJson(child_json);
        json["children"].push_back(child_json);
    }
}

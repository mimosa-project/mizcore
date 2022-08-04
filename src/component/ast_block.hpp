#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include "ast_component.hpp"
#include "ast_type.hpp"

namespace mizcore {

class ASTToken;
class KeywordToken;
class ASTStatement;

class ASTBlock : public ASTComponent
{
  public:
    // ctor, dtor
    ASTBlock(BLOCK_TYPE type)
      : block_type_(type)
    {}
    ~ASTBlock() override = default;
    ASTBlock(ASTBlock const&) = delete;
    ASTBlock(ASTBlock&&) = delete;
    ASTBlock& operator=(ASTBlock const&) = delete;
    ASTBlock& operator=(ASTBlock&&) = delete;

    // attributes
    ELEMENT_TYPE GetElementType() const override { return ELEMENT_TYPE::BLOCK; }

    BLOCK_TYPE GetBlockType() const { return block_type_; }
    void SetBlockType(BLOCK_TYPE block_type) { block_type_ = block_type; }

    ASTToken* GetFirstToken() const { return first_token_; }
    void SetFirstToken(ASTToken* token) { first_token_ = token; }
    ASTToken* GetLastToken() const { return last_token_; }
    void SetLastToken(ASTToken* token) { last_token_ = token; }
    ASTToken* GetSemicolonToken() const { return semicolon_token_; }
    void SetSemicolonToken(ASTToken* token) { semicolon_token_ = token; }

    ASTToken* GetRangeFirstToken() const override { return first_token_; }
    ASTToken* GetRangeLastToken() const override
    {
        return semicolon_token_ == nullptr ? last_token_ : semicolon_token_;
    }

    size_t GetChildComponentNum() const { return child_components_.size(); }
    ASTComponent* GetChildComponent(size_t i) const
    {
        return child_components_[i].get();
    }
    ASTBlock* GetChildBlock(size_t i) const;
    ASTStatement* GetChildStatement(size_t i) const;
    void AddChildComponent(std::unique_ptr<ASTComponent>&& component)
    {
        component->SetParent(this);
        child_components_.push_back(std::move(component));
    }
    void PopBackChildComponent() { child_components_.pop_back(); }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    BLOCK_TYPE block_type_ = BLOCK_TYPE::UNKNOWN;
    ASTToken* first_token_ = nullptr;
    ASTToken* last_token_ = nullptr;
    ASTToken* semicolon_token_ = nullptr;
    std::vector<std::unique_ptr<ASTComponent>> child_components_;
};

} // namespace mizcore

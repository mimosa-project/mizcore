#pragma once

#include <cstdint>

#include "ast_component.hpp"
#include "ast_type.hpp"

namespace mizcore {

class ASTToken;

class ASTStatement : public ASTComponent
{
  public:
    // ctor, dtor
    ASTStatement(STATEMENT_TYPE type)
      : statement_type_(type)
    {}
    ~ASTStatement() override = default;
    ASTStatement(ASTStatement const&) = delete;
    ASTStatement(ASTStatement&&) = delete;
    ASTStatement& operator=(ASTStatement const&) = delete;
    ASTStatement& operator=(ASTStatement&&) = delete;

    // attributes
    ELEMENT_TYPE GetElementType() const override
    {
        return ELEMENT_TYPE::STATEMENT;
    }

    STATEMENT_TYPE GetStatementType() const { return statement_type_; }
    void SetStatementType(STATEMENT_TYPE statement_type)
    {
        statement_type_ = statement_type;
    }

    ASTToken* GetRangeFirstToken() const override { return range_first_token_; }
    ASTToken* GetRangeLastToken() const override { return range_last_token_; }
    void SetRangeFirstToken(ASTToken* token) { range_first_token_ = token; }
    void SetRangeLastToken(ASTToken* token) { range_last_token_ = token; }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    STATEMENT_TYPE statement_type_ = STATEMENT_TYPE::UNKNOWN;
    ASTToken* range_first_token_ = nullptr;
    ASTToken* range_last_token_ = nullptr;
};

} // namespace mizcore

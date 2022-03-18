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

    ASTToken* GetRangeStartToken() const override { return range_start_token_; }
    ASTToken* GetRangeEndToken() const override { return range_end_token_; }
    void SetRangeStartToken(ASTToken* token) { range_start_token_ = token; }
    void SetRangeEndToken(ASTToken* token) { range_end_token_ = token; }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    STATEMENT_TYPE statement_type_ = STATEMENT_TYPE::UNKNOWN;
    ASTToken* range_start_token_ = nullptr;
    ASTToken* range_end_token_ = nullptr;
};

} // namespace mizcore

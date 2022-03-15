#pragma once

#include <cstdint>

#include "ast_component.hpp"
#include "ast_type.hpp"

namespace mizcore {

class Token;

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

    Token* GetRangeStartToken() const override { return range_start_token_; }
    Token* GetRangeEndToken() const override { return range_end_token_; }
    void SetRangeStartToken(Token* token) { range_start_token_ = token; }
    void SetRangeEndToken(Token* token) { range_end_token_ = token; }

    // operations
    void ToJson(nlohmann::json& json) const override;

  private:
    STATEMENT_TYPE statement_type_ = STATEMENT_TYPE::UNKNOWN;
    Token* range_start_token_ = nullptr;
    Token* range_end_token_ = nullptr;
};

} // namespace mizcore

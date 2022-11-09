#include <pybind11/pybind11.h>

#include "ast_statement.hpp"
#include "ast_type.hpp"

namespace mizcore {

class PyASTStatement : public ASTStatement
{
  public:
    using ASTStatement::ASTStatement;

    ELEMENT_TYPE GetElementType() const override
    {
      PYBIND11_OVERRIDE
      (
        ELEMENT_TYPE,
        ASTStatement,
        GetElementType,
      );
    }

    ASTToken* GetRangeFirstToken() const override
    {
      PYBIND11_OVERRIDE
      (
        ASTToken*,
        ASTStatement,
        GetRangeFirstToken,
      );
    }

    ASTToken* GetRangeLastToken() const override
    {
      PYBIND11_OVERRIDE
      (
        ASTToken*,
        ASTStatement,
        GetRangeLastToken,
      );
    }
};

} // namespace mizcore
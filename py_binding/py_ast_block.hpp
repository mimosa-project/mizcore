#include <pybind11/pybind11.h>

#include "ast_block.hpp"
#include "ast_type.hpp"

namespace mizcore {

class PyASTBlock : public ASTBlock
{
  public:
    using ASTBlock::ASTBlock;

    ELEMENT_TYPE GetElementType() const override
    {
      PYBIND11_OVERRIDE
      (
        ELEMENT_TYPE,
        ASTBlock,
        GetElementType,
      );
    }

    ASTToken* GetRangeFirstToken() const override
    {
      PYBIND11_OVERRIDE
      (
        ASTToken*,
        ASTBlock,
        GetRangeFirstToken,
      );
    }

    ASTToken* GetRangeLastToken() const override
    {
      PYBIND11_OVERRIDE
      (
        ASTToken*,
        ASTBlock,
        GetRangeLastToken,
      );
    }
};

} // namespace mizcore
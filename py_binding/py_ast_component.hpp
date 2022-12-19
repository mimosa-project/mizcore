#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_component.hpp"
#include "ast_type.hpp"
#include "ast_token.hpp"

namespace mizcore {

class PyASTComponent : public ASTComponent
{
  public:
    using ASTComponent::ASTComponent;

    ASTToken* GetRangeFirstToken() const override
    {
      PYBIND11_OVERRIDE_PURE
      (
        ASTToken*,
        ASTComponent,
        GetRangeFirstToken,
      );
    }

    ASTToken* GetRangeLastToken() const override
    {
      PYBIND11_OVERRIDE_PURE
      (
        ASTToken*,
        ASTComponent,
        GetRangeLastToken,
      );
    }
};

} // namespace mizcore

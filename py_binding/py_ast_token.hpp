#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class ASTToken;

class PyASTToken : public ASTToken
{
  public:
    using ASTToken::ASTToken;

    std::string_view GetText() const override
    {
      PYBIND11_OVERRIDE_PURE
      (
        std::string_view,
        ASTToken,
        GetText,
      );
    }

    TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE_PURE
      (
        TOKEN_TYPE,
        ASTToken,
        GetTokenType,
      );
    }

    IdentifierToken* GetRefToken() const override
    {
      PYBIND11_OVERRIDE_PURE
      (
        IdentifierToken*,
        ASTToken,
        GetRefToken,
      );
    }

    ELEMENT_TYPE GetElementType() const override
    {
      PYBIND11_OVERRIDE
      (
        ELEMENT_TYPE,
        ASTToken,
        GetElementType,
      );
    }
};

} // namespace mizcore

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
        get_text,
      );
    }

    TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE_PURE
      (
        TOKEN_TYPE,
        ASTToken,
        get_token_type,
      );
    }
};

} // namespace mizcore

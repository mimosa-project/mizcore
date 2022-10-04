#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class IdentifierToken;

class PyIdentifierToken : public IdentifierToken
{
  using IdentifierToken::IdentifierToken;

  std::string_view GetText() const override
  {
    PYBIND11_OVERRIDE
    (
      std::string_view,
      IdentifierToken,
      GetText,
    );
  }

  TOKEN_TYPE GetTokenType() const override
  {
    PYBIND11_OVERRIDE
    (
      TOKEN_TYPE,
      IdentifierToken,
      GetTokenType,
    );
  }

  IDENTIFIER_TYPE GetIdentifierType() const
  {
    PYBIND11_OVERRIDE
    (
      IDENTIFIER_TYPE,
      IdentifierToken,
      GetIdentifierType,
    );
  }

  IdentifierToken* GetRefToken() const
    {
      PYBIND11_OVERRIDE
      (
        IdentifierToken*,
        IdentifierToken,
        GetRefToken,
      );
    }
};

} // namespace mizcore
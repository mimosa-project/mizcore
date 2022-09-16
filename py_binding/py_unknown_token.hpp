#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class UnknownToken;

class PyUnknownToken : public UnknownToken
{
  using UnknownToken::UnknownToken;

  std::string_view GetText() const override
  {
    PYBIND11_OVERRIDE
    (
      std::string_view,
      UnknownToken,
      GetText,
    );
  }

  TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE
      (
        TOKEN_TYPE,
        UnknownToken,
        GetTokenType,
      );
    }
};

} // namespace mizcore
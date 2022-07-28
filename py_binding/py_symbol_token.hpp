#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class SymbolToken;

class PySymbolToken : public SymbolToken
{
  using SymbolToken::SymbolToken;

  std::string_view GetText() const override
  {
    PYBIND11_OVERRIDE
    (
      std::string_view,
      SymbolToken,
      GetText,
    );
  }

  TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE
      (
        TOKEN_TYPE,
        SymbolToken,
        GetTokenType,
      );
    }
};

} // namespace mizcore
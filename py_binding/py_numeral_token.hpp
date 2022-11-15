#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class NumeralToken;

class PyNumeralToken : public NumeralToken
{
  public:
    using NumeralToken::NumeralToken;

    std::string_view GetText() const override
    {
      PYBIND11_OVERRIDE
      (
        std::string_view,
        NumeralToken,
        GetText,
      );
    }

    TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE
      (
        TOKEN_TYPE,
        NumeralToken,
        GetTokenType,
      );
    }

    IdentifierToken* GetRefToken() const override
    {
      PYBIND11_OVERRIDE
      (
        IdentifierToken*,
        NumeralToken,
        GetRefToken,
      );
    }
};

} // namespace mizcore
#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class KeywordToken;

class PyKeywordToken : public KeywordToken
{
  public:
    using KeywordToken::KeywordToken;

    std::string_view GetText() const override
    {
      PYBIND11_OVERRIDE
      (
        std::string_view,
        KeywordToken,
        GetText,
      );
    }

    TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE
      (
        TOKEN_TYPE,
        KeywordToken,
        GetTokenType,
      );
    }

    IdentifierToken* GetRefToken() const override
    {
      PYBIND11_OVERRIDE
      (
        IdentifierToken*,
        KeywordToken,
        GetRefToken,
      );
    }
};

} // namespace mizcore
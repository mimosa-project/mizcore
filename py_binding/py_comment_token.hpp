#include <pybind11/pybind11.h>
#include <string_view>

#include "ast_token.hpp"
#include "ast_type.hpp"

namespace mizcore {

class CommentToken;

class PyCommentToken : public CommentToken
{
  public:
    using CommentToken::CommentToken;

    std::string_view GetText() const override
    {
      PYBIND11_OVERRIDE
      (
        std::string_view,
        CommentToken,
        GetText,
      );
    }

    TOKEN_TYPE GetTokenType() const override
    {
      PYBIND11_OVERRIDE
      (
        TOKEN_TYPE,
        CommentToken,
        GetTokenType,
      );
    }

    IdentifierToken* GetRefToken() const override
    {
      PYBIND11_OVERRIDE
      (
        IdentifierToken*,
        CommentToken,
        GetRefToken,
      );
    }
};

} // namespace mizcore
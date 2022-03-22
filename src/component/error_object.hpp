#pragma once

#include "error_def.hpp"

namespace mizcore {

class ASTElement;
class ASTToken;

class ErrorObject
{
  public:
    // ctor, dtor
    ErrorObject(ERROR_TYPE type, ASTToken* token)
      : error_type_(type)
      , ast_token_(token)
    {}
    virtual ~ErrorObject() = default;
    ErrorObject(ErrorObject const&) = default;
    ErrorObject(ErrorObject&&) = default;
    ErrorObject& operator=(ErrorObject const&) = default;
    ErrorObject& operator=(ErrorObject&&) = default;

    // attributes
    ERROR_TYPE GetErrorType() const { return error_type_; }
    ASTToken* GetASTToken() const { return ast_token_; }
    std::string GetMessage() const;

  private:
    ERROR_TYPE error_type_ = ERROR_TYPE::UNKNOWN;
    ASTToken* ast_token_ = nullptr;
};

} // namespace mizcore

#pragma once

#include "ast_type.hpp"

namespace mizcore {

class ASTElement
{
  public:
    ASTElement() = default;
    virtual ~ASTElement() = default;

    ASTElement(ASTElement const&) = delete;
    ASTElement(ASTElement&&) = delete;
    ASTElement& operator=(ASTElement const&) = delete;
    ASTElement& operator=(ASTElement&&) = delete;

    virtual ELEMENT_TYPE GetElementType() const = 0;
};

} // namespace mizcore

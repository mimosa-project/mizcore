#pragma once

#include "ast_type.hpp"
#include "nlohmann/json.hpp"

namespace mizcore {

class ASTElement
{
  public:
    // ctor, dtor
    ASTElement() = default;
    virtual ~ASTElement() = default;

    ASTElement(ASTElement const&) = delete;
    ASTElement(ASTElement&&) = delete;
    ASTElement& operator=(ASTElement const&) = delete;
    ASTElement& operator=(ASTElement&&) = delete;

    // attribute
    virtual ELEMENT_TYPE GetElementType() const = 0;

    // operations
    virtual void ToJson(nlohmann::json& json) const = 0;
};

} // namespace mizcore

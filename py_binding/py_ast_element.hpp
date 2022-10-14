#include <pybind11/pybind11.h>

#include "ast_element.hpp"
#include "ast_type.hpp"

namespace mizcore {

class PyASTElement : public ASTElement
{
  public:
    using ASTElement::ASTElement;

    // ELEMENT_TYPE GetElementType() const override
    // {
    //   PYBIND11_OVERRIDE_PURE
    //   (
    //     ELEMENT_TYPE,
    //     ASTElement,
    //     get_element_type,
    //   );
    // }
};

} // namespace mizcore
#pragma once

#include <memory>

namespace mizcore {

class ASTNode;

class ASTTree
{
  private:
    std::unique_ptr<ASTNode> root_node_;
};

} // namespace mizcore

#pragma once

#include <memory>

namespace emcore {

class ASTNode;

class ASTTree
{
  private:
    std::unique_ptr<ASTNode> root_node_;
};

} // namespace emcore

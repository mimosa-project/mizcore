#pragma once

#include <memory>

namespace emcore::mizar {

class ASTNode;

class ASTTree {
public:
    ASTTree() = default;
    virtual ~ASTTree() = default;
private:
    std::unique_ptr<ASTNode> root_node_;
};

}

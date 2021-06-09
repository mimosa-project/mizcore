#pragma once
#include <vector>

namespace mizcore {

class Token;

class ASTNode
{
  public:
    bool IsSentence() const { return children_.empty(); }

  private:
    std::vector<ASTNode*> children_;
    std::vector<Token*> tokens_;
};

} // namespace mizcore
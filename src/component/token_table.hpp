#pragma once

#include <memory>
#include <vector>

namespace emcore {

class Token;

class TokenTable
{
  public:
    // ctor, dtor
    TokenTable() = default;
    virtual ~TokenTable() = default;

    // attributes
    void AddToken(Token* token);
    Token* GetToken(size_t i) const { return tokens_[i].get(); }
    size_t GetTokenNum() const { return tokens_.size(); }

    // operations
    void Dump(std::ostream& os) const;

  private:
    std::vector<std::unique_ptr<Token>> tokens_;
};

} // namespace emcore

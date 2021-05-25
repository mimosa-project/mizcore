#pragma once

#include <memory>
#include <vector>

namespace emcore {

class Token;

class TokenArray
{
  public:
    // ctor, dtor
    TokenArray() = default;
    virtual ~TokenArray() = default;

    // attributes
    void AddToken(Token* token);
    Token* GetToken(size_t i) const { return tokens_[i].get(); }
    size_t GetTokenNum() const { return tokens_.size(); }
    std::vector<Token*> CollectTokenArray() const;

    // operations
    void Dump(std::ostream& os) const;

  private:
    std::vector<std::unique_ptr<Token>> tokens_;
};

} // namespace emcore

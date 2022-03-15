#pragma once

#include <memory>
#include <vector>

#include "nlohmann/json.hpp"

namespace mizcore {

class Token;

class TokenTable
{
  public:
    // ctor, dtor
    TokenTable() = default;
    virtual ~TokenTable() = default;
    TokenTable(TokenTable const&) = delete;
    TokenTable(TokenTable&&) = delete;
    TokenTable& operator=(TokenTable const&) = delete;
    TokenTable& operator=(TokenTable&&) = delete;

    // attributes
    void AddToken(Token* token);
    Token* GetToken(size_t i) const { return tokens_[i].get(); }
    size_t GetTokenNum() const { return tokens_.size(); }
    Token* GetLastToken() const
    {
        return tokens_.empty() ? nullptr : tokens_.back().get();
    }

    // operations
    void ToJson(nlohmann::json& json) const;

  private:
    std::vector<std::unique_ptr<Token>> tokens_;
};

} // namespace mizcore

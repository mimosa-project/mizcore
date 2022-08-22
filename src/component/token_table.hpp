#pragma once

#include <memory>
#include <vector>

#include "nlohmann/json.hpp"

namespace mizcore {

class ASTToken;

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
    void AddToken(ASTToken* token);
    void ReplaceToken(ASTToken* token, size_t i);
    ASTToken* GetToken(size_t i) const { return tokens_[i].get(); }
    size_t GetTokenNum() const { return tokens_.size(); }
    ASTToken* GetLastToken() const
    {
        return tokens_.empty() ? nullptr : tokens_.back().get();
    }

    // operations
    void ToJson(nlohmann::json& json) const;

  private:
    std::vector<std::unique_ptr<ASTToken>> tokens_;
};

} // namespace mizcore

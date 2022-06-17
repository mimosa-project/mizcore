#pragma once

#include <memory>

namespace mizcore {

class ASTBlock;
class TokenTable;
class ErrorTable;

class MizController
{
  public:
    void Exec(const char* mizpath);
    std::shared_ptr<TokenTable> GetTokenTable() const { return token_table_; }
    std::shared_ptr<ASTBlock> GetASTRoot() const { return ast_root_; }
    std::shared_ptr<ErrorTable> GetErrorTable() const { return error_table_; }

  private:
    std::shared_ptr<TokenTable> token_table_;
    std::shared_ptr<ASTBlock> ast_root_;
    std::shared_ptr<ErrorTable> error_table_;
};

} // namespace mizcore
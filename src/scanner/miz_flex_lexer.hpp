#pragma once

#include <memory>
#include <stack>

#include "ast_type.hpp"

namespace mizcore {

class ASTComponent;
class ASTBlock;
class ASTStatement;
class SymbolTable;
class ASTToken;
class TokenTable;

class MizFlexLexer : public yyMizFlexLexer
{
  public:
    MizFlexLexer(std::istream* in, std::shared_ptr<SymbolTable> symbol_table);
    virtual ~MizFlexLexer() = default;

    MizFlexLexer(const MizFlexLexer&) = delete;
    MizFlexLexer(MizFlexLexer&&) = delete;
    MizFlexLexer& operator=(const MizFlexLexer&) = delete;
    MizFlexLexer& operator=(MizFlexLexer&&) = delete;

    virtual int yylex();

    std::shared_ptr<TokenTable> GetTokenTable() const { return token_table_; }
    std::shared_ptr<SymbolTable> GetSymbolTable() const
    {
        return symbol_table_;
    }

  private:
    size_t ScanSymbol();
    size_t ScanIdentifier();
    size_t ScanKeyword(KEYWORD_TYPE type);
    size_t ScanNumeral();
    size_t ScanFileName();
    size_t ScanComment(COMMENT_TYPE token_type);
    size_t ScanUnknown();

  private:
    std::shared_ptr<SymbolTable> symbol_table_;
    std::shared_ptr<TokenTable> token_table_;
    size_t line_number_ = 1;
    size_t column_number_ = 1;

    bool is_in_environ_section_ = false;
    bool is_in_vocabulary_section_ = false;
};

} // namespace mizcore

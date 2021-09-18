#pragma once

#include <memory>
#include <stack>

#include "ast_type.hpp"
#include "error_def.hpp"
#include "token_type.hpp"

namespace mizcore {

class ASTBlock;
class ASTComponent;
class ASTStatement;
class IdentifierToken;
class Token;
class KeywordToken;
class TokenTable;

class MizBlockParser
{
  public:
    MizBlockParser(std::shared_ptr<TokenTable> token_table);
    virtual ~MizBlockParser() = default;
    MizBlockParser(MizBlockParser const&) = delete;
    MizBlockParser(MizBlockParser&&) = delete;
    MizBlockParser& operator=(MizBlockParser const&) = delete;
    MizBlockParser& operator=(MizBlockParser&&) = delete;

    std::shared_ptr<TokenTable> GetTokenTable() const { return token_table_; }
    std::shared_ptr<ASTBlock> GetASTRoot() const { return ast_root_; }

    bool IsPartialMode() const { return is_partial_mode_; }
    void SetPartialMode(bool is_partial_mode)
    {
        is_partial_mode_ = is_partial_mode;
    }

    void Parse();

  private:
    void ParseUnknown(Token* token);
    void ParseNumeral(Token* token);
    void ParseSymbol(Token* token, Token* prev_token);
    void ParseIdentifier(Token* token);
    void ParseKeyword(Token* token, Token* prev_token);

    void ParseEnvironKeyword(KeywordToken* token);
    void ParseBeginKeyword(KeywordToken* token);
    void ParseBlockStartKeyword(KeywordToken* token);
    void ParseSchemeKeyword(KeywordToken* token);
    void ParseProofKeyword(KeywordToken* token, Token* prev_token);
    void ParseEndKeyword(KeywordToken* token);
    void ParseKeywordDefault(KeywordToken* token);

    ASTBlock* PushBlock(Token* token,
                        ASTBlock* parent_block,
                        BLOCK_TYPE block_type);
    void PopBlock();
    ASTStatement* PushStatement(Token* token,
                                ASTBlock* parent_block,
                                STATEMENT_TYPE statement_type);
    void PopStatement(Token* token);
    void AddToStatement(Token* token);
    void AddEmptyStatement(Token* token, ASTBlock* parent_block);
    Token* QueryNextToken(Token* token) const;

    void ResolveIdentifierType(IdentifierToken* token);
    Token* ResolveLabelReference(IdentifierToken* label_token);
    Token* ResolveVariableReference(IdentifierToken* variable_token);
    void RecordError(Token* token, ERROR_TYPE error) const;

    ASTComponent* GetCurrentComponent() const
    {
        return ast_component_stack_.top();
    }

    static STATEMENT_TYPE GetStatementType(KEYWORD_TYPE keyword_type);
    static BLOCK_TYPE GetBlockType(KEYWORD_TYPE keyword_type);
    static bool IsBlockStartKeyword(KEYWORD_TYPE keyword_type);
    static ERROR_TYPE CheckBlockInBlockConsistency(BLOCK_TYPE inner_block_type,
                                                   BLOCK_TYPE outer_block_type);
    static ERROR_TYPE CheckStatementInBlockConsistency(
      STATEMENT_TYPE statement_type,
      BLOCK_TYPE block_type);
    static ERROR_TYPE CheckTokenInStatementConsistency(
      Token* token,
      STATEMENT_TYPE statement_type);
    static ERROR_TYPE CheckAdjacentTokensConsistency(Token* prev_token,
                                                     Token* current_token);

  private:
    bool is_partial_mode_ = false;
    std::shared_ptr<TokenTable> token_table_;
    std::shared_ptr<ASTBlock> ast_root_ =
      std::make_shared<ASTBlock>(BLOCK_TYPE::ROOT);
    std::stack<ASTComponent*> ast_component_stack_;

    // Only for internal use
    bool is_in_environ_ = false;
    bool is_in_section_ = false;
    Token* scheme_start_token_ = nullptr;
};

} // namespace mizcore

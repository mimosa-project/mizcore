#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

#include "spdlog/spdlog.h"

#include "ast_block.hpp"
#include "ast_statement.hpp"
#include "miz_block_parser.hpp"
#include "token.hpp"
#include "token_table.hpp"

using mizcore::ASTBlock;
using mizcore::ASTStatement;
using mizcore::MizBlockParser;
using mizcore::Token;

using mizcore::BLOCK_TYPE;
using mizcore::ELEMENT_TYPE;
using mizcore::ERROR_TYPE;
using mizcore::KEYWORD_TYPE;
using mizcore::STATEMENT_TYPE;
using mizcore::TOKEN_TYPE;

// [Purpose]
// - Build block and statement structure
// - Resolve identifier type
// - Resolve reference relations for labels and variables
// - Check block parsing errors
//
// [Error Check Policy]
//  - Check
//      1  Consistency of the block structure
//      2  Consistency between a block and its parent block.
//      3  Whether a variable is declared.
//      4  Whether a label is declared.
//  - Do not check (These are jobs of StatementParser)
//      5  Consistency between a statement and its parent block.
//      6  Consistency between a token and its belonging statement.
//      7  Consistency of token order.
//
// [TODO]
//  - Detect obvious errors before calling StatementParser

MizBlockParser::MizBlockParser(std::shared_ptr<TokenTable> token_table)
  : token_table_(std::move(token_table))
{
    ast_component_stack_.push(ast_root_.get());
}

void
MizBlockParser::Parse()
{
    Token* prev_token = nullptr;
    size_t token_num = token_table_->GetTokenNum();
    for (size_t i = 0; i < token_num; ++i) {
        auto* token = token_table_->GetToken(i);
        auto token_type = token->GetTokenType();
        switch (token_type) {
            case TOKEN_TYPE::UNKNOWN:
                ParseUnknown(token);
                break;
            case TOKEN_TYPE::NUMERAL:
                ParseNumeral(token);
                break;
            case TOKEN_TYPE::SYMBOL:
                ParseSymbol(token, prev_token);
                break;
            case TOKEN_TYPE::IDENTIFIER:
                ParseIdentifier(token);
                break;
            case TOKEN_TYPE::KEYWORD:
                ParseKeyword(token, prev_token);
                break;
            case TOKEN_TYPE::COMMENT:
                // ignore
                break;
        }

        if (token_type != TOKEN_TYPE::COMMENT) {
            prev_token = token;
        }
    }

    // Check the last token of the article
    if (ast_component_stack_.size() != 1) {
        auto* component = GetCurrentComponent();
        auto element_type = component->GetElementType();
        if (element_type == ELEMENT_TYPE::BLOCK) {
            RecordError(prev_token, ERROR_TYPE::BLOCK_NOT_CLOSED_IN_ARTICLE);
        } else {
            RecordError(prev_token,
                        ERROR_TYPE::STATEMENT_NOT_CLOSED_IN_ARTICLE);
        }
    }
}

void
MizBlockParser::ParseUnknown(Token* token)
{
    // The error will be detected in StatementParser.
    auto* component = GetCurrentComponent();
    assert(component != nullptr);

    if (component->GetElementType() == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::UNKNOWN);
    }
    RecordError(token, ERROR_TYPE::TOKEN_IS_UNKNOWN);
}

void
MizBlockParser::ParseNumeral(Token* token)
{
    auto* component = GetCurrentComponent();
    assert(component != nullptr);

    if (component->GetElementType() == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::UNKNOWN);
    }
}

void
MizBlockParser::ParseSymbol(Token* token, Token* prev_token)
{
    auto* component = GetCurrentComponent();
    assert(component != nullptr);

    auto element_type = component->GetElementType();
    if (token->GetText() == ";") {
        if (element_type == ELEMENT_TYPE::BLOCK) {
            auto* block = static_cast<ASTBlock*>(component);
            if (prev_token != nullptr && prev_token->GetText() == "end" &&
                block->GetBlockType() != BLOCK_TYPE::ROOT) {
                block->SetSemicolonToken(token);
                PopBlock();
            } else {
                // The error will be detected in StatementParser
                PushStatement(token, block, STATEMENT_TYPE::EMPTY);
                PopStatement(token);
            }
        } else {
            PopStatement(token);
        }
    } else if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* block = static_cast<ASTBlock*>(component);
        PushStatement(token, block, STATEMENT_TYPE::UNKNOWN);
    }
}

void
MizBlockParser::ParseIdentifier(Token* token)
{
    auto* component = GetCurrentComponent();
    assert(component != nullptr);

    if (component->GetElementType() == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::UNKNOWN);
    }
}

void
MizBlockParser::ParseKeyword(Token* token, Token* prev_token)
{
    auto* keyword_token = static_cast<KeywordToken*>(token);
    auto keyword_type = keyword_token->GetKeywordType();

    switch (keyword_type) {
        case KEYWORD_TYPE::ENVIRON:
            ParseEnvironKeyword(keyword_token);
            break;
        case KEYWORD_TYPE::BEGIN_:
            ParseBeginKeyword(keyword_token);
            break;
        case KEYWORD_TYPE::DEFINITION:
        case KEYWORD_TYPE::REGISTRATION:
        case KEYWORD_TYPE::NOTATION:
        case KEYWORD_TYPE::CASE:
        case KEYWORD_TYPE::SUPPOSE:
        case KEYWORD_TYPE::HEREBY:
        case KEYWORD_TYPE::NOW:
            ParseBlockStartKeyword(keyword_token);
            break;
        case KEYWORD_TYPE::SCHEME:
            ParseSchemeKeyword(keyword_token);
            break;
        case KEYWORD_TYPE::PROOF:
            ParseProofKeyword(keyword_token, prev_token);
            break;
        case KEYWORD_TYPE::END:
            ParseEndKeyword(keyword_token);
            break;
        default:
            ParseKeywordDefault(keyword_token);
            break;
    }
}

void
MizBlockParser::ParseEnvironKeyword(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::ENVIRON);
        PopStatement(token);

        is_in_environ_ = true;
        if (is_in_section_) {
            RecordError(token, ERROR_TYPE::ENVIRON_KEYWORD_AFTER_BEGIN_KEYWORD);
            is_in_section_ = false;
        }
    } else {
        RecordError(token, ERROR_TYPE::ENVIRON_KEYWORD_IN_STATEMENT);
    }
}

void
MizBlockParser::ParseBeginKeyword(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::SECTION);
        PopStatement(token);

        is_in_section_ = true;
        is_in_environ_ = false;
    } else {
        RecordError(token, ERROR_TYPE::BEGIN_KEYWORD_IN_STATEMENT);
    }
}

void
MizBlockParser::ParseBlockStartKeyword(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        auto keyword_type = token->GetKeywordType();
        PushBlock(token, parent_block, QueryBlockType(keyword_type));
    } else {
        RecordError(token, ERROR_TYPE::BLOCK_START_KEYWORD_IN_STATEMENT);
    }
}

void
MizBlockParser::ParseSchemeKeyword(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);

        // Read tokens until a semicolon or "proof" keyword
        size_t token_num = token_table_->GetTokenNum();
        size_t id = token->GetId() + 1;
        for (; id < token_num; ++id) {
            Token* current_token = token_table_->GetToken(id);
            if (current_token->GetText() == ";") {
                PushBlock(token, parent_block, BLOCK_TYPE::SCHEME);
                break;
            }
            if (current_token->GetText() == "proof") {
                PushStatement(token, parent_block, STATEMENT_TYPE::SCHEME);
                break;
            }
        }

        if (id == token_num) {
            // The error (No closed statement) will be detected in Parse()
            PushStatement(token, parent_block, STATEMENT_TYPE::SCHEME);
        }
    } else {
        RecordError(token, ERROR_TYPE::SCHEME_KEYWORD_IN_STATEMENT);
    }
}

void
MizBlockParser::ParseProofKeyword(KeywordToken* token, Token* prev_token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        // Error will be detected in CheckBlockSiblingsConsistency()
        PushBlock(token, parent_block, BLOCK_TYPE::PROOF);
        ++proof_stack_num_;
    } else {
        PopStatement(prev_token);
        if (prev_token == nullptr || prev_token->GetText() == ";") {
            RecordError(token, ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION);
        }
        auto* parent_component = ast_component_stack_.top();
        assert(parent_component->GetElementType() == ELEMENT_TYPE::BLOCK);
        auto* parent_block = static_cast<ASTBlock*>(parent_component);
        PushBlock(token, parent_block, BLOCK_TYPE::PROOF);
    }
}

void
MizBlockParser::ParseEndKeyword(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* block = static_cast<ASTBlock*>(component);
        if (block->GetBlockType() == BLOCK_TYPE::ROOT) {
            // The error will be detected in StatementParser.
            PushStatement(token, block, STATEMENT_TYPE::UNKNOWN);
        } else {
            block->SetEndToken(token);
            Token* next_token = QueryNextToken(token);
            if (next_token == nullptr || next_token->GetText() != ";") {
                RecordError(token, ERROR_TYPE::END_KEYWORD_WITHOUT_SEMICOLON);
                PopBlock();
            }
        }
    } else {
        RecordError(token, ERROR_TYPE::END_KEYWORD_IN_STATEMENT);
    }
}

void
MizBlockParser::ParseKeywordDefault(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        auto keyword_type = token->GetKeywordType();
        PushStatement(token, parent_block, QueryStatementType(keyword_type));
    }
}

ASTBlock*
MizBlockParser::PushBlock(Token* token,
                          ASTBlock* parent_block,
                          BLOCK_TYPE block_type)
{
    // Create a new block and set it to a parent.
    std::unique_ptr<ASTBlock> block = std::make_unique<ASTBlock>(block_type);
    block->SetFirstToken(token);
    ast_component_stack_.push(block.get());

    ASTBlock* raw_block = block.get();
    parent_block->AddChildComponent(std::move(block));

    auto parent_block_type = parent_block->GetBlockType();
    ERROR_TYPE error =
      CheckBlockInBlockConsistency(block_type, parent_block_type);
    if (error != ERROR_TYPE::SUCCESS) {
        RecordError(token, error);
    }

    error = CheckBlockSiblingsConsistency(raw_block, parent_block);
    if (error != ERROR_TYPE::SUCCESS) {
        RecordError(token, error);
    }

    return raw_block;
}

void
MizBlockParser::PopBlock()
{
    auto* component = GetCurrentComponent();
    assert(component != nullptr);
    assert(component->GetElementType() == ELEMENT_TYPE::BLOCK);
    assert(ast_component_stack_.size() > 1);

    auto* block = static_cast<ASTBlock*>(component);
    if (block->GetBlockType() == BLOCK_TYPE::PROOF) {
        assert(proof_stack_num_ > 0);
        --proof_stack_num_;
    }
    ast_component_stack_.pop();
}

ASTStatement*
MizBlockParser::PushStatement(Token* token,
                              ASTBlock* parent_block,
                              STATEMENT_TYPE statement_type)
{
    // Create a new statement and set it to a parent block.
    std::unique_ptr<ASTStatement> statement =
      std::make_unique<ASTStatement>(statement_type);
    statement->SetRangeStartToken(token);
    ast_component_stack_.push(statement.get());

    ASTStatement* raw_statement = statement.get();
    parent_block->AddChildComponent(std::move(statement));

    return raw_statement;
}

void
MizBlockParser::PopStatement(Token* token)
{
    auto* component = GetCurrentComponent();
    assert(GetCurrentComponent() != nullptr);
    assert(GetCurrentComponent()->GetElementType() == ELEMENT_TYPE::STATEMENT);
    auto* statement = static_cast<ASTStatement*>(component);
    statement->SetRangeEndToken(token);

    assert(ast_component_stack_.size() > 1);
    ast_component_stack_.pop();
}

Token*
MizBlockParser::QueryNextToken(Token* token) const
{
    size_t token_num = token_table_->GetTokenNum();
    for (size_t i = token->GetId(); i < token_num; ++i) {
        auto* next_token = token_table_->GetToken(i);
        if (next_token->GetTokenType() != TOKEN_TYPE::COMMENT) {
            return next_token;
        }
    }
    return nullptr;
}

void
MizBlockParser::ResolveIdentifierType(IdentifierToken* token)
{
    // TODO(nakasho): Not implemented yet.
}

Token*
MizBlockParser::ResolveLabelReference(IdentifierToken* label_token)
{
    // TODO(nakasho): Not implemented yet.
    return nullptr;
}

Token*
MizBlockParser::ResolveVariableReference(IdentifierToken* variable_token)
{
    // TODO(nakasho): Not implemented yet.
    return nullptr;
}

void
MizBlockParser::RecordError(Token* token, ERROR_TYPE error) const
{
    // TODO(nakasho): Change error log report.
    assert(token != nullptr);
    auto error_level = GetErrorLevel(error);
    spdlog::error("[{}] ({%4d},{%3d}): \"{}\": C{}:{}",
                  GetErrorLevelText(error_level),
                  token->GetLineNumber(),
                  token->GetColumnNumber(),
                  token->GetText(),
                  GetErrorCode(error),
                  GetErrorMessage(error));
    if (error_level == ERROR_LEVEL::ERROR) {
        GetCurrentComponent()->SetError(true);
    }
}

ERROR_TYPE
MizBlockParser::CheckBlockInBlockConsistency(BLOCK_TYPE inner_block_type,
                                             BLOCK_TYPE outer_block_type) const
{
    switch (inner_block_type) {
        case BLOCK_TYPE::ROOT:
            return ERROR_TYPE::ROOT_BLOCK_UNDER_BLOCK;
        case BLOCK_TYPE::DEFINITION:
        case BLOCK_TYPE::REGISTRATION:
        case BLOCK_TYPE::NOTATION:
        case BLOCK_TYPE::SCHEME:
            if (outer_block_type != BLOCK_TYPE::ROOT) {
                return ERROR_TYPE::NOT_JUST_BELOW_ROOT_BLOCK;
            } else if (!is_partial_mode_ && !is_in_section_) {
                return ERROR_TYPE::NOT_AFTER_BEGIN_KEYWORD;
            }
            break;
        case BLOCK_TYPE::CASE:
        case BLOCK_TYPE::SUPPOSE:
        case BLOCK_TYPE::NOW:
        case BLOCK_TYPE::HEREBY:
            if (proof_stack_num_ < 1) {
                return ERROR_TYPE::NOT_UNDER_PROOF_BLOCK;
            }
            break;
        default:
            break;
    }

    return ERROR_TYPE::SUCCESS;
}

ERROR_TYPE
MizBlockParser::CheckBlockSiblingsConsistency(ASTBlock* block,
                                              ASTBlock* parent_block)
{
    auto block_type = block->GetBlockType();
    switch (block_type) {
        case BLOCK_TYPE::CASE:
        case BLOCK_TYPE::SUPPOSE: {
            size_t child_num = parent_block->GetChildComponentNum();
            if (child_num < 2) {
                return ERROR_TYPE::PER_CASES_STATEMENT_MISSING;
            }
            auto* first_sibling_statement = parent_block->GetChildStatement(0);
            if (first_sibling_statement == nullptr ||
                first_sibling_statement->GetStatementType() !=
                  STATEMENT_TYPE::PER_CASES) {
                return ERROR_TYPE::PER_CASES_STATEMENT_MISSING;
            }

            if (child_num > 2) {
                auto* prev_sibling_block =
                  parent_block->GetChildBlock(child_num - 2);
                if (prev_sibling_block == nullptr ||
                    prev_sibling_block->GetBlockType() != block_type) {
                    return (block_type == BLOCK_TYPE::CASE)
                             ? ERROR_TYPE::
                                 CASE_BLOCK_PREV_SIBLING_NOT_CASE_BLOCK
                             : ERROR_TYPE::
                                 SUPPOSE_BLOCK_PREV_SIBLING_NOT_SUPPOSE_BLOCK;
                }
            }
        } break;
        case BLOCK_TYPE::PROOF: {
            size_t child_num = parent_block->GetChildComponentNum();
            if (child_num < 2) {
                return ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION;
            }
            auto* prev_sibling_statement =
              parent_block->GetChildStatement(child_num - 2);
            if (prev_sibling_statement == nullptr) {
                return ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION;
            }
            auto prev_sibling_type = prev_sibling_statement->GetStatementType();
            if (prev_sibling_type != STATEMENT_TYPE::UNKNOWN &&
                prev_sibling_type != STATEMENT_TYPE::SCHEME &&
                prev_sibling_type != STATEMENT_TYPE::THEOREM) {
                return ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION;
            }
            const auto* token = prev_sibling_statement->GetRangeEndToken();
            if (token->GetText() == ";") {
                return ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION;
            }
        } break;
        default:
            break;
    }
    return ERROR_TYPE::SUCCESS;
}

STATEMENT_TYPE
MizBlockParser::QueryStatementType(KEYWORD_TYPE keyword_type)
{
    switch (keyword_type) {
        case KEYWORD_TYPE::ENVIRON:
            return STATEMENT_TYPE::ENVIRON;
        case KEYWORD_TYPE::BEGIN_:
            return STATEMENT_TYPE::SECTION;
        case KEYWORD_TYPE::VOCABULARIES:
            return STATEMENT_TYPE::VOCABULARIES;
        case KEYWORD_TYPE::NOTATIONS:
            return STATEMENT_TYPE::NOTATIONS;
        case KEYWORD_TYPE::CONSTRUCTORS:
            return STATEMENT_TYPE::CONSTRUCTORS;
        case KEYWORD_TYPE::REGISTRATIONS:
            return STATEMENT_TYPE::REGISTRATIONS;
        case KEYWORD_TYPE::DEFINITIONS:
            return STATEMENT_TYPE::DEFINITIONS;
        case KEYWORD_TYPE::EXPANSIONS:
            return STATEMENT_TYPE::EXPANSIONS;
        case KEYWORD_TYPE::EQUALITIES:
            return STATEMENT_TYPE::EQUALITIES;
        case KEYWORD_TYPE::THEOREMS:
            return STATEMENT_TYPE::THEOREMS;
        case KEYWORD_TYPE::SCHEMES:
            return STATEMENT_TYPE::SCHEMES;
        case KEYWORD_TYPE::REQUIREMENTS:
            return STATEMENT_TYPE::REQUIREMENTS;
        default:
            return STATEMENT_TYPE::UNKNOWN;
    }
}

BLOCK_TYPE
MizBlockParser::QueryBlockType(KEYWORD_TYPE keyword_type)
{
    switch (keyword_type) {
        case KEYWORD_TYPE::DEFINITION:
            return BLOCK_TYPE::DEFINITION;
        case KEYWORD_TYPE::REGISTRATION:
            return BLOCK_TYPE::REGISTRATION;
        case KEYWORD_TYPE::NOTATION:
            return BLOCK_TYPE::NOTATION;
        case KEYWORD_TYPE::SCHEME:
            return BLOCK_TYPE::SCHEME;
        case KEYWORD_TYPE::CASE:
            return BLOCK_TYPE::CASE;
        case KEYWORD_TYPE::SUPPOSE:
            return BLOCK_TYPE::SUPPOSE;
        case KEYWORD_TYPE::HEREBY:
            return BLOCK_TYPE::HEREBY;
        case KEYWORD_TYPE::NOW:
            return BLOCK_TYPE::NOW;
        case KEYWORD_TYPE::PROOF:
            return BLOCK_TYPE::PROOF;
        default:
            return BLOCK_TYPE::UNKNOWN;
    }
}

#include <cassert>
#include <iostream>
#include <memory>
#include <sstream>

#include "spdlog/spdlog.h"

#include "ast_block.hpp"
#include "ast_statement.hpp"
#include "ast_token.hpp"
#include "error_object.hpp"
#include "error_table.hpp"
#include "miz_block_parser.hpp"
#include "token_table.hpp"

using mizcore::ASTBlock;
using mizcore::ASTStatement;
using mizcore::ASTToken;
using mizcore::MizBlockParser;

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

MizBlockParser::MizBlockParser(std::shared_ptr<TokenTable> token_table,
                               std::shared_ptr<ErrorTable> error_table)
  : token_table_(std::move(token_table))
  , error_table_(std::move(error_table))
{
    ast_component_stack_.push(ast_root_.get());
    PushReferenceStack();
}

void
MizBlockParser::Parse()
{
    ASTToken* prev_token = nullptr;
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

    // Resolve identifier type and references
    ResolveIdentifierInBlock(ast_root_.get());
}

void
MizBlockParser::ParseUnknown(ASTToken* token)
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
MizBlockParser::ParseNumeral(ASTToken* token)
{
    auto* component = GetCurrentComponent();
    assert(component != nullptr);

    if (component->GetElementType() == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::UNKNOWN);
    }
}

void
MizBlockParser::ParseSymbol(ASTToken* token, ASTToken* prev_token)
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
MizBlockParser::ParseIdentifier(ASTToken* token)
{
    auto* component = GetCurrentComponent();
    assert(component != nullptr);

    if (component->GetElementType() == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        PushStatement(token, parent_block, STATEMENT_TYPE::UNKNOWN);
    }
}

void
MizBlockParser::ParseKeyword(ASTToken* token, ASTToken* prev_token)
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
            ParseBlockStartKeyword(keyword_token);
            break;
        case KEYWORD_TYPE::NOW:
            ParseNowKeyword(keyword_token);
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
MizBlockParser::ParseNowKeyword(KeywordToken* token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        auto keyword_type = token->GetKeywordType();
        PushBlock(token, parent_block, QueryBlockType(keyword_type));
    } else {
        // sometimes now keyword accompanies label declaration (A1:) and "thus" before.
        // Diffuse-Statement = [ Label-Identifier ":" ] "now" Reasoning "end" ";" .
        // Diffuse-Conclusion = "thus" Diffuse-Statement | "hereby" Reasoning "end" ";" .
        // We do not check the preceding tokens here.
        // This check will be done in ResolveNowBlockIdentifier.
        auto* current_statement = static_cast<ASTStatement*>(component);
        auto* statement_first_token = current_statement->GetRangeFirstToken();
        auto* parent_block = current_statement->GetParent();
        parent_block->PopBackChildComponent();
        ast_component_stack_.pop();
        PushBlock(statement_first_token, parent_block, BLOCK_TYPE::NOW);
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
            ASTToken* current_token = token_table_->GetToken(id);
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
MizBlockParser::ParseProofKeyword(KeywordToken* token, ASTToken* prev_token)
{
    auto* component = GetCurrentComponent();
    auto element_type = component->GetElementType();
    if (element_type == ELEMENT_TYPE::BLOCK) {
        auto* parent_block = static_cast<ASTBlock*>(component);
        // Error will be detected in CheckBlockSiblingsConsistency()
        PushBlock(token, parent_block, BLOCK_TYPE::PROOF);
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
            block->SetLastToken(token);
            ASTToken* next_token = QueryNextToken(token);
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
MizBlockParser::PushBlock(ASTToken* token,
                          ASTBlock* parent_block,
                          BLOCK_TYPE block_type)
{
    // Create a new block and set it to a parent.
    std::unique_ptr<ASTBlock> block = std::make_unique<ASTBlock>(block_type);
    block->SetFirstToken(token);
    ast_component_stack_.push(block.get());

    ASTBlock* raw_block = block.get();
    parent_block->AddChildComponent(std::move(block));

    if (block_type == BLOCK_TYPE::PROOF) {
        ++proof_stack_num_;
    }

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
MizBlockParser::PushStatement(ASTToken* token,
                              ASTBlock* parent_block,
                              STATEMENT_TYPE statement_type)
{
    // Create a new statement and set it to a parent block.
    std::unique_ptr<ASTStatement> statement =
      std::make_unique<ASTStatement>(statement_type);
    statement->SetRangeFirstToken(token);
    ast_component_stack_.push(statement.get());

    ASTStatement* raw_statement = statement.get();
    parent_block->AddChildComponent(std::move(statement));

    return raw_statement;
}

void
MizBlockParser::PopStatement(ASTToken* token)
{
    auto* component = GetCurrentComponent();
    assert(GetCurrentComponent() != nullptr);
    assert(GetCurrentComponent()->GetElementType() == ELEMENT_TYPE::STATEMENT);
    auto* statement = static_cast<ASTStatement*>(component);
    statement->SetRangeLastToken(token);

    assert(ast_component_stack_.size() > 1);
    ast_component_stack_.pop();
}

ASTToken*
MizBlockParser::QueryNextToken(ASTToken* token) const
{
    size_t token_num = token_table_->GetTokenNum();
    for (size_t i = token->GetId() + 1; i < token_num; ++i) {
        auto* next_token = token_table_->GetToken(i);
        if (next_token->GetTokenType() != TOKEN_TYPE::COMMENT) {
            return next_token;
        }
    }
    return nullptr;
}

void
MizBlockParser::ResolveIdentifierInBlock(ASTBlock* block)
{
    PushReferenceStack();
    if (block->GetBlockType() == BLOCK_TYPE::NOW) {
        ResolveNowBlockIdentifier(block);
    }

    size_t child_num = block->GetChildComponentNum();
    for (size_t i = 0; i < child_num; ++i) {
        auto* component = block->GetChildComponent(i);
        if (component->GetElementType() == ELEMENT_TYPE::BLOCK) {
            ResolveIdentifierInBlock(static_cast<ASTBlock*>(component));
        } else {
            ResolveIdentifierInStatement(static_cast<ASTStatement*>(component));
        }
    }
    PopReferenceStack();
}

void
MizBlockParser::ResolveIdentifierInStatement(ASTStatement* statement)
{
    auto* first_token = statement->GetRangeFirstToken();
    auto* last_token = statement->GetRangeLastToken();

    size_t first_id = first_token->GetId();
    size_t last_id = last_token->GetId();

    KEYWORD_TYPE region_type = KEYWORD_TYPE::UNKNOWN;
    bool is_scheme_parameters = false;
    size_t scheme_bracket_stack = 0;

    for (size_t i = first_id; i < last_id; ++i) {
        ASTToken* curr_token = token_table_->GetToken(i);
        ASTToken* prev_token =
          i > first_id ? token_table_->GetToken(i - 1) : nullptr;
        ASTToken* next_token = token_table_->GetToken(i + 1);

        if (curr_token->GetTokenType() == TOKEN_TYPE::KEYWORD) {
            auto* keyword_token = static_cast<KeywordToken*>(curr_token);
            auto keyword_type = keyword_token->GetKeywordType();

            // check region start condition
            if (region_type == KEYWORD_TYPE::UNKNOWN) {
                if (i == first_id) {
                    switch (keyword_type) {
                        case KEYWORD_TYPE::RESERVE:
                        case KEYWORD_TYPE::GIVEN:
                        case KEYWORD_TYPE::LET:
                        case KEYWORD_TYPE::RECONSIDER:
                        case KEYWORD_TYPE::SET:
                        case KEYWORD_TYPE::SCHEME:
                        case KEYWORD_TYPE::DEFPRED:
                        case KEYWORD_TYPE::DEFFUNC:
                            region_type = keyword_type;
                            continue;
                        default:
                            break;
                    }
                }
                switch (keyword_type) {
                    case KEYWORD_TYPE::FOR:
                    case KEYWORD_TYPE::EX:
                    case KEYWORD_TYPE::WHERE:
                    case KEYWORD_TYPE::MEANS:
                    case KEYWORD_TYPE::EQUALS:
                    case KEYWORD_TYPE::BY:
                    case KEYWORD_TYPE::FROM:
                        region_type = keyword_type;
                        continue;
                    default:
                        continue;
                }
            }
        }

        // reserved identifier
        if (region_type == KEYWORD_TYPE::RESERVE) {
            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "reserve" || prev_text == ",") {
                if (next_text == "," || next_text == "for") {
                    curr_token = ReplaceIdentifierType(
                      curr_token, IDENTIFIER_TYPE::RESERVED);
                    PushToReferenceStack(curr_token);
                }
            }
        }

        // variable identifier
        if (region_type == KEYWORD_TYPE::FOR ||
            region_type == KEYWORD_TYPE::EX) {
            const auto& curr_text = curr_token->GetText();
            if (curr_text == "st" || curr_text == "hold" ||
                curr_text == "for" || curr_text == "ex" || curr_text == "be" ||
                curr_text == "being") {
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "for" || prev_text == "ex" || prev_text == ",") {
                if (next_text == "st" || next_text == "holds" ||
                    next_text == "for" || next_text == "ex" ||
                    next_text == "be" || next_text == "being" ||
                    next_text == ",") {
                    ReplaceIdentifierType(curr_token,
                                          IDENTIFIER_TYPE::VARIABLE);
                }
            }
        }

        if (region_type == KEYWORD_TYPE::GIVEN ||
            region_type == KEYWORD_TYPE::LET) {
            const auto& curr_text = curr_token->GetText();
            if (curr_text == "such" || curr_text == "be" ||
                curr_text == "being" || curr_text == ";") {
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "given" || prev_text == "let" ||
                prev_text == ",") {
                if (next_text == "such" || next_text == "be" ||
                    next_text == "being" || next_text == ";" ||
                    next_text == ",") {
                    curr_token = ReplaceIdentifierType(
                      curr_token, IDENTIFIER_TYPE::VARIABLE);
                    PushToReferenceStack(curr_token);
                }
            }
        }

        if (region_type == KEYWORD_TYPE::RECONSIDER) {
            const auto& curr_text = curr_token->GetText();
            if (curr_text == "as") {
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "reconsider" || prev_text == ",") {
                if (next_text == "=" || next_text == "," || next_text == "as") {
                    curr_token = ReplaceIdentifierType(
                      curr_token, IDENTIFIER_TYPE::VARIABLE);
                    PushToReferenceStack(curr_token);
                }
            }
        }

        if (region_type == KEYWORD_TYPE::SET) {
            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "set" || prev_text == ",") {
                if (next_text == "=") {
                    curr_token = ReplaceIdentifierType(
                      curr_token, IDENTIFIER_TYPE::VARIABLE);
                    PushToReferenceStack(curr_token);
                }
            }
        }

        if (region_type == KEYWORD_TYPE::WHERE) {
            const auto& curr_text = curr_token->GetText();
            if (curr_text == "is" || curr_text == "are") {
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "where" || prev_text == ",") {
                if (next_text == "," || next_text == "is" ||
                    next_text == "are") {
                    ReplaceIdentifierType(curr_token,
                                          IDENTIFIER_TYPE::VARIABLE);
                }
            }
        }

        // label identifier
        if (region_type == KEYWORD_TYPE::MEANS ||
            region_type == KEYWORD_TYPE::EQUALS) {
            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == ":" && next_text == ":") {
                ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::LABEL);
                PushToReferenceStack(curr_token);
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }
        }

        if (i == first_id) {
            const auto& next_text = next_token->GetText();
            if (next_text == ":") {
                curr_token =
                  ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::LABEL);
                PushToReferenceStack(curr_token);
            }
        }

        if (prev_token != nullptr) {
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();

            if (next_text == ":") {
                if (prev_text == "that" || prev_text == "and" ||
                    prev_text == "provided" || prev_text == "case" ||
                    prev_text == "suppose" || prev_text == "assume" ||
                    prev_text == "theorem" || prev_text == "thus" ||
                    prev_text == "hence" || prev_text == "then") {
                    curr_token =
                      ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::LABEL);
                    PushToReferenceStack(curr_token);
                }
            }
        }

        if (region_type == KEYWORD_TYPE::BY) {
            const auto& curr_text = curr_token->GetText();
            if (curr_text == ";" || curr_text == ".=") {
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "by" || prev_text == ",") {
                if (next_text == "," || next_text == ";" || next_text == ".=") {
                    curr_token =
                      ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::LABEL);
                }
            }
        }

        if (region_type == KEYWORD_TYPE::FROM) {
            const auto& curr_text = curr_token->GetText();
            if (curr_text == ")") {
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "(" || prev_text == ",") {
                if (next_text == ")" || next_text == ",") {
                    curr_token =
                      ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::LABEL);
                }
            }
            // scheme identifier
            if (prev_text == "from" && next_text == "(") {
                curr_token =
                  ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::SCHEME);
            }
        }

        // scheme identifier
        if (region_type == KEYWORD_TYPE::SCHEME) {
            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "scheme" && next_text == "{") {
                curr_token =
                  ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::SCHEME);
                PushToReferenceStack(curr_token);
            }

            const auto& curr_text = curr_token->GetText();
            if (curr_text == "{") {
                is_scheme_parameters = true;
            } else if (curr_text == "}") {
                is_scheme_parameters = false;
                region_type = KEYWORD_TYPE::UNKNOWN;
                continue;
            }

            if (is_scheme_parameters) {
                if (scheme_bracket_stack == 0 &&
                    (prev_text == "{" || prev_text == ",")) {
                    if (next_text == "[") {
                        curr_token = ReplaceIdentifierType(
                          curr_token, IDENTIFIER_TYPE::PREDICATE);
                        PushToReferenceStack(curr_token);
                    } else if (next_text == "(") {
                        curr_token = ReplaceIdentifierType(
                          curr_token, IDENTIFIER_TYPE::FUNCTOR);
                        PushToReferenceStack(curr_token);
                    } else if (next_text == ",") {
                        size_t next_id = next_token->GetId();
                        for (size_t j = next_id + 1; j < last_id; ++j) {
                            auto* look_ahead_token = token_table_->GetToken(j);
                            auto look_ahead_text = look_ahead_token->GetText();
                            if (look_ahead_text == "[") {
                                curr_token = ReplaceIdentifierType(
                                  curr_token, IDENTIFIER_TYPE::PREDICATE);
                                PushToReferenceStack(curr_token);
                                break;
                            }

                            if (look_ahead_text == "(") {
                                curr_token = ReplaceIdentifierType(
                                  curr_token, IDENTIFIER_TYPE::FUNCTOR);
                                PushToReferenceStack(curr_token);
                                break;
                            }
                        }
                    }
                }

                if (curr_text == "(" || curr_text == "[") {
                    ++scheme_bracket_stack;
                } else if (curr_text == ")" || curr_text == "]") {
                    --scheme_bracket_stack;
                }
            }
        }

        // predicate identifier
        if (region_type == KEYWORD_TYPE::DEFPRED) {
            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "defpred" && next_text == "[") {
                curr_token =
                  ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::PREDICATE);
                PushToReferenceStack(curr_token);
            }
        }

        // functor identifier
        if (region_type == KEYWORD_TYPE::DEFFUNC) {
            assert(prev_token != nullptr);
            const auto& prev_text = prev_token->GetText();
            const auto& next_text = next_token->GetText();
            if (prev_text == "deffunc" && next_text == "(") {
                curr_token =
                  ReplaceIdentifierType(curr_token, IDENTIFIER_TYPE::FUNCTOR);
                PushToReferenceStack(curr_token);
            }
        }

        ResolveReference(curr_token);
    }
}

void
MizBlockParser::ResolveNowBlockIdentifier(ASTBlock* block)
{
    // Diffuse-Statement = [ Label-Identifier ":" ] "now" Reasoning "end" ";" .
    // Diffuse-Conclusion = "thus" Diffuse-Statement | "hereby" Reasoning "end" ";" .
    auto* t0 = block->GetRangeFirstToken();

    std::vector<ASTToken*> pre_tokens;
    for (size_t i = t0->GetId();; ++i) {
        ASTToken* token = token_table_->GetToken(i);
        if (token->GetTokenType() == TOKEN_TYPE::KEYWORD &&
            static_cast<KeywordToken*>(token)->GetKeywordType() ==
              KEYWORD_TYPE::NOW) {
            break;
        }
        pre_tokens.push_back(token);
    }

    if (pre_tokens.size() > 3) {
        RecordError(t0, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        return;
    }

    if (pre_tokens.size() == 1) {
        if (!IsThusToken(t0)) {
            RecordError(t0, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        }
    } else if (pre_tokens.size() == 2) {
        auto* t1 = pre_tokens[1];
        if (!CanBeLabelToken(t0)) {
            RecordError(t0, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        } else if (!IsSemicolonToken(t1)) {
            RecordError(t1, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        } else {
            ReplaceIdentifierType(t0, IDENTIFIER_TYPE::LABEL);
        }
    } else if (pre_tokens.size() == 3) {
        auto* t1 = pre_tokens[1];
        auto* t2 = pre_tokens[2];
        if (!IsThusToken(t0)) {
            RecordError(t0, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        } else if (!CanBeLabelToken(t1)) {
            RecordError(t1, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        } else if (!IsSemicolonToken(t2)) {
            RecordError(t2, ERROR_TYPE::NOW_BLOCK_STARTS_WITH_IRREGULAR_TOKENS);
        } else {
            ReplaceIdentifierType(t1, IDENTIFIER_TYPE::LABEL);
        }
    }
}

void
MizBlockParser::RecordError(ASTToken* token, ERROR_TYPE error_type) const
{
    assert(token != nullptr);

    auto* error = new ErrorObject(error_type, token);
    error_table_->AddError(error);

    auto error_level = GetErrorLevel(error_type);
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
            const auto* token = prev_sibling_statement->GetRangeLastToken();
            if (token->GetText() == ";") {
                return ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION;
            }
        } break;
        default:
            break;
    }
    return ERROR_TYPE::SUCCESS;
}

bool
MizBlockParser::IsThusToken(ASTToken* token)
{
    return token->GetTokenType() == TOKEN_TYPE::KEYWORD &&
           static_cast<KeywordToken*>(token)->GetKeywordType() ==
             KEYWORD_TYPE::THUS;
}

bool
MizBlockParser::IsSemicolonToken(ASTToken* token)
{
    return token->GetTokenType() == TOKEN_TYPE::SYMBOL &&
           token->GetText() == std::string(";");
}

bool
MizBlockParser::CanBeLabelToken(ASTToken* token) const
{
    if (token->GetTokenType() == TOKEN_TYPE::IDENTIFIER) {
        return true;
    }
    if (is_partial_mode_ && token->GetTokenType() == TOKEN_TYPE::SYMBOL) {
        auto text = token->GetText();
        assert(!text.empty());

        if (isalpha(text[0]) == 0) {
            return false;
        }

        for (size_t i = 1; i < text.size(); ++i) {
            if ((isalnum(text[i]) == 0) || text[i] != '_') {
                return false;
            }
        }
        return true;
    }

    return false;
}

ASTToken*
MizBlockParser::ReplaceIdentifierType(ASTToken* token, IDENTIFIER_TYPE type)
{
    if (token->GetTokenType() == TOKEN_TYPE::IDENTIFIER) {
        static_cast<IdentifierToken*>(token)->SetIdentifierType(
          IDENTIFIER_TYPE::LABEL);
        return token;
    }

    if (is_partial_mode_ && token->GetTokenType() == TOKEN_TYPE::SYMBOL) {
        size_t token_id = token->GetId();
        size_t line_number = token->GetLineNumber();
        size_t column_number = token->GetColumnNumber();
        std::string_view text = token->GetText();

        ASTToken* new_token =
          new IdentifierToken(line_number, column_number, text, type);
        assert(token);
        token_table_->ReplaceToken(new_token, token_id);
        return new_token;
    }

    return token;
}

void
MizBlockParser::PushReferenceStack()
{
    reference_stack_.emplace_back(std::vector<IdentifierToken*>());
}

void
MizBlockParser::PopReferenceStack()
{
    reference_stack_.pop_back();
}

void
MizBlockParser::PushToReferenceStack(ASTToken* token)
{
    assert(!reference_stack_.empty());
    assert(token->GetTokenType() == TOKEN_TYPE::IDENTIFIER);
    reference_stack_.back().push_back(static_cast<IdentifierToken*>(token));
}

void
MizBlockParser::ResolveReference(ASTToken* token)
{
    auto token_type = token->GetTokenType();
    if (token_type != TOKEN_TYPE::IDENTIFIER &&
        (!is_partial_mode_ || token_type != TOKEN_TYPE::SYMBOL)) {
        return;
    }

    std::string_view text = token->GetText();
    for (auto rit = reference_stack_.rbegin(); rit != reference_stack_.rend();
         ++rit) {
        for (auto rit2 = rit->rbegin(); rit2 != rit->rend(); ++rit2) {
            IdentifierToken* ref_token = *rit2;
            if (ref_token == token) {
                continue;
            }

            std::string_view ref_text = ref_token->GetText();
            if (text == ref_text) {
                ReplaceIdentifierType(token, ref_token->GetIdentifierType());
                assert(token->GetTokenType() == TOKEN_TYPE::IDENTIFIER);
                static_cast<IdentifierToken*>(token)->SetRefToken(ref_token);
                return;
            }
        }
    }
}

#include "miz_pattern_parser.hpp"
#include "ast_block.hpp"
#include "ast_statement.hpp"
#include "ast_token.hpp"
#include "ast_type.hpp"
#include "error_table.hpp"
#include "pattern_element.hpp"
#include "pattern_table.hpp"
#include "symbol_table.hpp"
#include "token_table.hpp"
#include <limits>

using mizcore::KeywordToken;
using mizcore::MizPatternParser;

using mizcore::KEYWORD_TYPE;
using mizcore::TOKEN_TYPE;

void
MizPatternParser::ParseBlock(const ASTBlock* ast_block,
                             const std::shared_ptr<TokenTable>& token_table,
                             const std::string& filename)
{
    size_t child_num = ast_block->GetChildComponentNum();
    for (size_t i = 0; i < child_num; ++i) {
        ASTComponent* child_component = ast_block->GetChildComponent(i);
        auto component_type = child_component->GetElementType();
        if (component_type == ELEMENT_TYPE::BLOCK) {
            ParseBlock(
              static_cast<ASTBlock*>(child_component), token_table, filename);
        } else if (component_type == ELEMENT_TYPE::STATEMENT) {
            ParseStatement(static_cast<ASTStatement*>(child_component),
                           token_table,
                           filename);
        }
    }
}

void
MizPatternParser::ParseStatement(ASTStatement* ast_statement,
                                 const std::shared_ptr<TokenTable>& token_table,
                                 const std::string& filename)
{
    auto* first_token = ast_statement->GetRangeFirstToken();
    auto* last_token = ast_statement->GetRangeLastToken();
    if (first_token->GetTokenType() != TOKEN_TYPE::KEYWORD) {
        return;
    }
    if (last_token->GetId() - first_token->GetId() < 3) {
        // too short statement
        return;
    }

    switch (static_cast<KeywordToken*>(first_token)->GetKeywordType()) {
        case KEYWORD_TYPE::ATTR: {
            ParseAttributeStatement(ast_statement, token_table, filename);
            break;
        }
        case KEYWORD_TYPE::FUNC: {
            auto* next_token = token_table->GetToken(first_token->GetId() + 1);
            if (next_token->GetTokenType() == TOKEN_TYPE::SYMBOL &&
                static_cast<SymbolToken*>(next_token)->GetSymbolType() ==
                  SYMBOL_TYPE::LEFT_FUNCTOR_BRACKET) {
                ParseBracketFunctorStatement(
                  ast_statement, token_table, filename);
            } else {
                ParseFunctorStatement(ast_statement, token_table, filename);
            }
            break;
        }
        case KEYWORD_TYPE::MODE: {
            ParseModeStatement(ast_statement, token_table, filename);
            break;
        }
        case KEYWORD_TYPE::PRED: {
            ParsePredicateStatement(ast_statement, token_table, filename);
            break;
        }
        case KEYWORD_TYPE::SYNONYM: {
            ParseSynonymStatement(ast_statement, token_table, filename);
            break;
        }
        case KEYWORD_TYPE::ANTONYM: {
            ParseAntonymStatement(ast_statement, token_table, filename);
            break;
        }
        default: {
            break;
        }
    }
}

// [Attribute Grammar]
// Attribute-Definition = "attr" Attribute-Pattern "means" Definiens ";" Correctness-Conditions .
// Attribute-Pattern = Locus "is" [ Attribute-Loci ] Attribute-Symbol .
// Attribute-Loci = Loci | "(" Loci ")" .
// Loci = Locus { "," Locus } .
// Locus = Variable-Identifier .
// Attribute-Symbol = Symbol .
void
MizPatternParser::ParseAttributeStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    uint8_t arity = 0;
    uint8_t parenthesis_num = 0;
    SymbolToken* symbol_token = nullptr;
    PARSING_TOKEN_TYPE prev_token_type = PARSING_TOKEN_TYPE::UNKNOWN;
    PARSING_TOKEN_TYPE current_token_type = PARSING_TOKEN_TYPE::ATTR;

    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        prev_token_type = current_token_type;
        current_token_type = GetParsingTokenType(token);
        switch (current_token_type) {
            case PARSING_TOKEN_TYPE::IDENTIFIER: {
                if (prev_token_type == PARSING_TOKEN_TYPE::ATTR) {
                    continue;
                }
                if (prev_token_type == PARSING_TOKEN_TYPE::IS ||
                    prev_token_type == PARSING_TOKEN_TYPE::COMMA ||
                    prev_token_type == PARSING_TOKEN_TYPE::LEFT_PARENTHESIS) {
                    ++arity;
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::IS: {
                if (id == first_id + 2) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::COMMA: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::LEFT_PARENTHESIS: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IS && arity == 0 &&
                    parenthesis_num == 0) {
                    ++parenthesis_num;
                    continue;
                }
                // illegal parenthesis position
                return;
            }
            case PARSING_TOKEN_TYPE::RIGHT_PARENTHESIS: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER &&
                    arity > 0 && parenthesis_num == 1) {
                    --parenthesis_num;
                    continue;
                }
                // illegal parenthesis position
                return;
            }
            case PARSING_TOKEN_TYPE::ATTRIBUTE_SYMBOL: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IS ||
                    prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER ||
                    prev_token_type == PARSING_TOKEN_TYPE::RIGHT_PARENTHESIS) {
                    if (parenthesis_num == 0) {
                        symbol_token = static_cast<SymbolToken*>(token);
                        continue;
                    }
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::MEANS:
            case PARSING_TOKEN_TYPE::FOR: {
                if (prev_token_type == PARSING_TOKEN_TYPE::ATTRIBUTE_SYMBOL) {
                    if (symbol_token != nullptr) {
                        // success
                        std::unique_ptr<PatternElement> arity_element =
                          std::make_unique<PatternElement>(filename);
                        arity_element->SetAttributePattern(symbol_token, arity);
                        arity_table_->AddPatternElement(
                          std::move(arity_element));
                    }
                }
                return;
            }
            default: {
                // illegal token
                return;
            }
        }
    }
}

// [Functor Grammar]
// Functor-Definition = "func" Functor-Pattern [ Specification ] [ ( "means" | "equals" ) Definiens ] ";"
//                           Correctness-Conditions { Functor-Property } .
// Functor-Pattern = [ Functor-Loci ] Functor-Symbol [ Functor-Loci ]
//                 | Left-Functor-Bracket Loci Right-Functor-Bracket .
// Specification = "->" Type-Expression .
// Functor-Symbol = Symbol .
// Functor-Loci = Locus | "(" Loci ")" .
// Loci = Locus { "," Locus } .
// Locus = Variable-Identifier .
// Left-Functor-Bracket = Symbol | "{" | "[" .
// Right-Functor-Bracket = Symbol | "}" | "]" .
void
MizPatternParser::ParseFunctorStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    uint8_t left_arity = 0;
    uint8_t right_arity = 0;
    uint8_t parenthesis_num = 0;
    SymbolToken* symbol_token = nullptr;
    PARSING_TOKEN_TYPE prev_token_type = PARSING_TOKEN_TYPE::UNKNOWN;
    PARSING_TOKEN_TYPE current_token_type = PARSING_TOKEN_TYPE::FUNC;

    bool is_parsing_left_arity = true;
    bool is_parsing_right_arity = false;

    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        prev_token_type = current_token_type;
        current_token_type = GetParsingTokenType(token);
        switch (current_token_type) {
            case PARSING_TOKEN_TYPE::IDENTIFIER: {
                if (prev_token_type == PARSING_TOKEN_TYPE::FUNC ||
                    prev_token_type == PARSING_TOKEN_TYPE::LEFT_PARENTHESIS ||
                    prev_token_type == PARSING_TOKEN_TYPE::COMMA ||
                    prev_token_type == PARSING_TOKEN_TYPE::FUNCTOR_SYMBOL) {
                    if (is_parsing_left_arity) {
                        ++left_arity;
                    } else if (is_parsing_right_arity) {
                        ++right_arity;
                    }
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::COMMA: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER &&
                    parenthesis_num == 1) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::LEFT_PARENTHESIS: {
                if (prev_token_type == PARSING_TOKEN_TYPE::FUNC ||
                    (prev_token_type == PARSING_TOKEN_TYPE::FUNCTOR_SYMBOL &&
                     parenthesis_num == 0)) {
                    ++parenthesis_num;
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::RIGHT_PARENTHESIS: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER &&
                    parenthesis_num == 1) {
                    --parenthesis_num;
                    is_parsing_left_arity = false;
                    is_parsing_right_arity = false;
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::FUNCTOR_SYMBOL: {
                if ((prev_token_type == PARSING_TOKEN_TYPE::RIGHT_PARENTHESIS &&
                     left_arity > 0) ||
                    (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER &&
                     left_arity == 1) ||
                    (prev_token_type == PARSING_TOKEN_TYPE::FUNC &&
                     left_arity == 0)) {
                    if (parenthesis_num == 0) {
                        is_parsing_left_arity = false;
                        is_parsing_right_arity = true;
                        symbol_token = static_cast<SymbolToken*>(token);
                        continue;
                    }
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::MEANS:
            case PARSING_TOKEN_TYPE::EQUALS:
            case PARSING_TOKEN_TYPE::SEMICOLON:
            case PARSING_TOKEN_TYPE::ARROW:
            case PARSING_TOKEN_TYPE::FOR: {
                if (prev_token_type == PARSING_TOKEN_TYPE::FUNCTOR_SYMBOL ||
                    prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER ||
                    prev_token_type == PARSING_TOKEN_TYPE::RIGHT_PARENTHESIS) {
                    if (parenthesis_num == 0 && symbol_token != nullptr) {
                        // success
                        std::unique_ptr<PatternElement> arity_element =
                          std::make_unique<PatternElement>(filename);
                        arity_element->SetFunctorPattern(
                          symbol_token, left_arity, right_arity);
                        arity_table_->AddPatternElement(
                          std::move(arity_element));
                    }
                }
                return;
            }
            default: {
                // illegal token
                return;
            }
        }
    }
}

// [Bracket Functor Grammar]
// Functor-Definition = "func" Functor-Pattern [ Specification ] [ ( "means" | "equals" ) Definiens ] ";"
//                           Correctness-Conditions { Functor-Property } .
// Functor-Pattern = [ Functor-Loci ] Functor-Symbol [ Functor-Loci ]
//                 | Left-Functor-Bracket Loci Right-Functor-Bracket .
// Specification = "->" Type-Expression .
// Functor-Symbol = Symbol .
// Functor-Loci = Locus | "(" Loci ")" .
// Loci = Locus { "," Locus } .
// Locus = Variable-Identifier .
// Left-Functor-Bracket = Symbol | "{" | "[" .
// Right-Functor-Bracket = Symbol | "}" | "]" .
void
MizPatternParser::ParseBracketFunctorStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    uint8_t arity = 0;
    SymbolToken* left_symbol_token = nullptr;
    SymbolToken* right_symbol_token = nullptr;
    PARSING_TOKEN_TYPE prev_token_type = PARSING_TOKEN_TYPE::UNKNOWN;
    PARSING_TOKEN_TYPE current_token_type = PARSING_TOKEN_TYPE::FUNC;

    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        prev_token_type = current_token_type;
        current_token_type = GetParsingTokenType(token);
        switch (current_token_type) {
            case PARSING_TOKEN_TYPE::IDENTIFIER: {
                if (prev_token_type ==
                      PARSING_TOKEN_TYPE::LEFT_FUNCTOR_BRACKET_SYMBOL ||
                    prev_token_type == PARSING_TOKEN_TYPE::COMMA) {
                    ++arity;
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::COMMA: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::LEFT_FUNCTOR_BRACKET_SYMBOL: {
                if (prev_token_type == PARSING_TOKEN_TYPE::FUNC) {
                    left_symbol_token = static_cast<SymbolToken*>(token);
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::RIGHT_FUNCTOR_BRACKET_SYMBOL: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER ||
                    prev_token_type ==
                      PARSING_TOKEN_TYPE::LEFT_FUNCTOR_BRACKET_SYMBOL) {
                    if (left_symbol_token != nullptr) {
                        // success
                        right_symbol_token = static_cast<SymbolToken*>(token);
                        std::unique_ptr<PatternElement> arity_element =
                          std::make_unique<PatternElement>(filename);
                        arity_element->SetBracketFunctorPattern(
                          left_symbol_token, right_symbol_token, arity);
                        arity_table_->AddPatternElement(
                          std::move(arity_element));
                    }
                }
                return;
            }
            default: {
                // illegal token
                return;
            }
        }
    }
}

// [Mode Grammar]
// Mode-Definition = "mode" Mode-Pattern ( [ Specification ] [ "means" Definiens ] ";"
//                   Correctness-Conditions | "is" Type-Expression ";" ) { Mode-Property } .
// Mode-Pattern = Mode-Symbol [ "of" Loci ] .
// Specification = "->" Type-Expression .
// Mode-Symbol = Symbol | "set" .
// Loci = Locus { "," Locus } .
// Locus = Variable-Identifier .
void
MizPatternParser::ParseModeStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    uint8_t arity = 0;
    SymbolToken* symbol_token = nullptr;
    PARSING_TOKEN_TYPE prev_token_type = PARSING_TOKEN_TYPE::UNKNOWN;
    PARSING_TOKEN_TYPE current_token_type = PARSING_TOKEN_TYPE::MODE;

    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        prev_token_type = current_token_type;
        current_token_type = GetParsingTokenType(token);
        switch (current_token_type) {
            case PARSING_TOKEN_TYPE::IDENTIFIER: {
                if (prev_token_type == PARSING_TOKEN_TYPE::OF ||
                    prev_token_type == PARSING_TOKEN_TYPE::COMMA) {
                    ++arity;
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::COMMA: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::OF: {
                if (prev_token_type == PARSING_TOKEN_TYPE::MODE_SYMBOL &&
                    arity == 0) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::MODE_SYMBOL: {
                if (prev_token_type == PARSING_TOKEN_TYPE::MODE) {
                    symbol_token = static_cast<SymbolToken*>(token);
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::ARROW:
            case PARSING_TOKEN_TYPE::MEANS:
            case PARSING_TOKEN_TYPE::SEMICOLON:
            case PARSING_TOKEN_TYPE::IS:
            case PARSING_TOKEN_TYPE::FOR: {
                if (prev_token_type == PARSING_TOKEN_TYPE::MODE_SYMBOL ||
                    prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    if (symbol_token != nullptr) {
                        // success
                        std::unique_ptr<PatternElement> arity_element =
                          std::make_unique<PatternElement>(filename);
                        arity_element->SetModePattern(symbol_token, arity);
                        arity_table_->AddPatternElement(
                          std::move(arity_element));
                    }
                }
                return;
            }
            default: {
                // illegal token
                return;
            }
        }
    }
}

// [Predicate Grammar]
// Predicate-Definition = "pred" Predicate-Pattern [ "means" Definiens ] ";"
//                        Correctness-Conditions { Predicate-Property } .
// Predicate-Pattern = [ Loci ] Predicate-Symbol [ Loci ] .
// Predicate-Symbol = Symbol | "=" .
// Loci = Locus { "," Locus } .
// Locus = Variable-Identifier .
void
MizPatternParser::ParsePredicateStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    uint8_t left_arity = 0;
    uint8_t right_arity = 0;
    SymbolToken* symbol_token = nullptr;
    PARSING_TOKEN_TYPE prev_token_type = PARSING_TOKEN_TYPE::UNKNOWN;
    PARSING_TOKEN_TYPE current_token_type = PARSING_TOKEN_TYPE::PRED;
    bool is_parsing_left_arity = true;

    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        prev_token_type = current_token_type;
        current_token_type = GetParsingTokenType(token);
        switch (current_token_type) {
            case PARSING_TOKEN_TYPE::IDENTIFIER: {
                if (prev_token_type == PARSING_TOKEN_TYPE::PRED ||
                    prev_token_type == PARSING_TOKEN_TYPE::COMMA ||
                    prev_token_type == PARSING_TOKEN_TYPE::PREDICATE_SYMBOL) {
                    is_parsing_left_arity ? ++left_arity : ++right_arity;
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::COMMA: {
                if (prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::PREDICATE_SYMBOL: {
                if (prev_token_type == PARSING_TOKEN_TYPE::PRED ||
                    prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    is_parsing_left_arity = false;
                    symbol_token = static_cast<SymbolToken*>(token);
                    continue;
                }
                // illegal position
                return;
            }
            case PARSING_TOKEN_TYPE::MEANS:
            case PARSING_TOKEN_TYPE::SEMICOLON:
            case PARSING_TOKEN_TYPE::FOR: {
                if (prev_token_type == PARSING_TOKEN_TYPE::PREDICATE_SYMBOL ||
                    prev_token_type == PARSING_TOKEN_TYPE::IDENTIFIER) {
                    if (symbol_token != nullptr) {
                        // succcess
                        std::unique_ptr<PatternElement> arity_element =
                          std::make_unique<PatternElement>(filename);
                        arity_element->SetPredicatePattern(
                          symbol_token, left_arity, right_arity);
                        arity_table_->AddPatternElement(
                          std::move(arity_element));
                    }
                }
                return;
            }
            default: {
                // illegal token
                return;
            }
        }
    }
}

// [Synonym Grammar]
// Mode-Synonym = "synonym" Mode-Pattern "for" Mode-Pattern ";" .
// Functor-Synonym = "synonym" Functor-Pattern "for" Functor-Pattern ";" .
// Predicate-Synonym = "synonym" Predicate-Pattern "for" Predicate-Pattern ";" .
// Attribute-Synonym = "synonym" Attribute-Pattern "for" Attribute-Pattern ";" .
void
MizPatternParser::ParseSynonymStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    // At first check which type of symbol is parsing,
    // and call each type of parse function.
    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        PARSING_TOKEN_TYPE token_type = GetParsingTokenType(token);
        switch (token_type) {
            case PARSING_TOKEN_TYPE::ATTRIBUTE_SYMBOL: {
                ParseAttributeStatement(ast_statement, token_table, filename);
                return;
            }
            case PARSING_TOKEN_TYPE::FUNCTOR_SYMBOL: {
                ParseFunctorStatement(ast_statement, token_table, filename);
                return;
            }
            case PARSING_TOKEN_TYPE::LEFT_FUNCTOR_BRACKET_SYMBOL: {
                ParseBracketFunctorStatement(
                  ast_statement, token_table, filename);
                return;
            }
            case PARSING_TOKEN_TYPE::MODE_SYMBOL: {
                ParseModeStatement(ast_statement, token_table, filename);
                return;
            }
            case PARSING_TOKEN_TYPE::PREDICATE_SYMBOL: {
                ParsePredicateStatement(ast_statement, token_table, filename);
                return;
            }
            default: {
                break;
            }
        }
    }
}

// [Antonym Grammar]
// Predicate-Antonym = "antonym" Predicate-Pattern "for" Predicate-Pattern ";" .
// Attribute-Antonym = "antonym" Attribute-Pattern "for" Attribute-Pattern ";" .
void
MizPatternParser::ParseAntonymStatement(
  ASTStatement* ast_statement,
  const std::shared_ptr<TokenTable>& token_table,
  const std::string& filename)
{
    size_t first_id = ast_statement->GetRangeFirstToken()->GetId();
    size_t last_id = ast_statement->GetRangeLastToken()->GetId();

    // At first check which type of symbol is parsing,
    // and call each type of parse function.
    for (size_t id = first_id + 1; id <= last_id; ++id) {
        auto* token = token_table->GetToken(id);
        PARSING_TOKEN_TYPE token_type = GetParsingTokenType(token);
        switch (token_type) {
            case PARSING_TOKEN_TYPE::ATTRIBUTE_SYMBOL: {
                ParseAttributeStatement(ast_statement, token_table, filename);
                return;
            }
            case PARSING_TOKEN_TYPE::PREDICATE_SYMBOL: {
                ParsePredicateStatement(ast_statement, token_table, filename);
                return;
            }
            default: {
                break;
            }
        }
    }
}

MizPatternParser::PARSING_TOKEN_TYPE
MizPatternParser::GetParsingTokenType(ASTToken* token)
{
    switch (token->GetTokenType()) {
        case TOKEN_TYPE::SYMBOL: {
            auto* symbol_token = static_cast<SymbolToken*>(token);
            switch (symbol_token->GetSymbolType()) {
                case SYMBOL_TYPE::ATTRIBUTE:
                    return PARSING_TOKEN_TYPE::ATTRIBUTE_SYMBOL;
                case SYMBOL_TYPE::FUNCTOR:
                    return PARSING_TOKEN_TYPE::FUNCTOR_SYMBOL;
                case SYMBOL_TYPE::LEFT_FUNCTOR_BRACKET:
                    return PARSING_TOKEN_TYPE::LEFT_FUNCTOR_BRACKET_SYMBOL;
                case SYMBOL_TYPE::RIGHT_FUNCTOR_BRACKET:
                    return PARSING_TOKEN_TYPE::RIGHT_FUNCTOR_BRACKET_SYMBOL;
                case SYMBOL_TYPE::MODE:
                    return PARSING_TOKEN_TYPE::MODE_SYMBOL;
                case SYMBOL_TYPE::PREDICATE:
                    return PARSING_TOKEN_TYPE::PREDICATE_SYMBOL;
                case SYMBOL_TYPE::SPECIAL:
                    switch (symbol_token->GetSpecialSymbolType()) {
                        case SPECIAL_SYMBOL_TYPE::COMMA:
                            return PARSING_TOKEN_TYPE::COMMA;
                        case SPECIAL_SYMBOL_TYPE::SEMICOLON:
                            return PARSING_TOKEN_TYPE::SEMICOLON;
                        case SPECIAL_SYMBOL_TYPE::ARROW:
                            return PARSING_TOKEN_TYPE::ARROW;
                        case SPECIAL_SYMBOL_TYPE::LEFT_PARENTHESIS:
                            return PARSING_TOKEN_TYPE::LEFT_PARENTHESIS;
                        case SPECIAL_SYMBOL_TYPE::RIGHT_PARENTHESIS:
                            return PARSING_TOKEN_TYPE::RIGHT_PARENTHESIS;
                        default:
                            return PARSING_TOKEN_TYPE::UNKNOWN;
                    }
                default:
                    return PARSING_TOKEN_TYPE::UNKNOWN;
            }
        }
        case TOKEN_TYPE::KEYWORD: {
            switch (static_cast<KeywordToken*>(token)->GetKeywordType()) {
                case KEYWORD_TYPE::IS:
                    return PARSING_TOKEN_TYPE::IS;
                case KEYWORD_TYPE::MEANS:
                    return PARSING_TOKEN_TYPE::MEANS;
                case KEYWORD_TYPE::EQUALS:
                    return PARSING_TOKEN_TYPE::EQUALS;
                case KEYWORD_TYPE::OF:
                    return PARSING_TOKEN_TYPE::OF;
                case KEYWORD_TYPE::FUNC:
                    return PARSING_TOKEN_TYPE::FUNC;
                case KEYWORD_TYPE::ATTR:
                    return PARSING_TOKEN_TYPE::ATTR;
                case KEYWORD_TYPE::MODE:
                    return PARSING_TOKEN_TYPE::MODE;
                case KEYWORD_TYPE::PRED:
                    return PARSING_TOKEN_TYPE::PRED;
                case KEYWORD_TYPE::SYNONYM:
                    return PARSING_TOKEN_TYPE::SYNONYM;
                case KEYWORD_TYPE::ANTONYM:
                    return PARSING_TOKEN_TYPE::ANTONYM;
                case KEYWORD_TYPE::FOR:
                    return PARSING_TOKEN_TYPE::FOR;
                default:
                    return PARSING_TOKEN_TYPE::UNKNOWN;
            }
        }
        case TOKEN_TYPE::IDENTIFIER:
            return PARSING_TOKEN_TYPE::IDENTIFIER;
        default:
            return PARSING_TOKEN_TYPE::UNKNOWN;
    }
    return PARSING_TOKEN_TYPE::UNKNOWN;
}

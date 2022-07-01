#pragma once

#include <memory>

#include "ast_type.hpp"
#include "error_def.hpp"

namespace mizcore {

class ASTToken;
class ErrorTable;
class TokenTable;
class PatternTable;
class PatternElement;
class SymbolTable;
class SymbolToken;
class ASTBlock;
class ASTStatement;

class MizPatternParser
{
  public:
    MizPatternParser() { error_table_ = std::make_shared<ErrorTable>(); }
    MizPatternParser(std::shared_ptr<ErrorTable> error_table)
      : error_table_(std::move(error_table))
    {}
    virtual ~MizPatternParser() = default;
    MizPatternParser(MizPatternParser const&) = delete;
    MizPatternParser(MizPatternParser&&) = delete;
    MizPatternParser& operator=(MizPatternParser const&) = delete;
    MizPatternParser& operator=(MizPatternParser&&) = delete;

    std::shared_ptr<PatternTable> GetArityTable() const { return arity_table_; }

    void ParseBlock(const ASTBlock* ast_block,
                    const std::shared_ptr<TokenTable>& token_table,
                    const std::string& filename);
    void ParseStatement(ASTStatement* ast_statement,
                        const std::shared_ptr<TokenTable>& token_table,
                        const std::string& filename);

  private:
    void ParseAttributeStatement(ASTStatement* ast_statement,
                                 const std::shared_ptr<TokenTable>& token_table,
                                 const std::string& filename);
    void ParseFunctorStatement(ASTStatement* ast_statement,
                               const std::shared_ptr<TokenTable>& token_table,
                               const std::string& filename);
    void ParseBracketFunctorStatement(
      ASTStatement* ast_statement,
      const std::shared_ptr<TokenTable>& token_table,
      const std::string& filename);
    void ParseModeStatement(ASTStatement* ast_statement,
                            const std::shared_ptr<TokenTable>& token_table,
                            const std::string& filename);
    void ParsePredicateStatement(ASTStatement* ast_statement,
                                 const std::shared_ptr<TokenTable>& token_table,
                                 const std::string& filename);
    void ParseSynonymStatement(ASTStatement* ast_statement,
                               const std::shared_ptr<TokenTable>& token_table,
                               const std::string& filename);
    void ParseAntonymStatement(ASTStatement* ast_statement,
                               const std::shared_ptr<TokenTable>& token_table,
                               const std::string& filename);

  private:
    enum class PARSING_TOKEN_TYPE
    {
        UNKNOWN,
        // special symbol
        COMMA,
        SEMICOLON,
        ARROW,
        LEFT_PARENTHESIS,
        RIGHT_PARENTHESIS,
        // symbol
        ATTRIBUTE_SYMBOL,
        FUNCTOR_SYMBOL,
        LEFT_FUNCTOR_BRACKET_SYMBOL,
        RIGHT_FUNCTOR_BRACKET_SYMBOL,
        MODE_SYMBOL,
        PREDICATE_SYMBOL,
        // keyword
        IS,
        MEANS,
        EQUALS,
        OF,
        FUNC,
        ATTR,
        MODE,
        PRED,
        SYNONYM,
        ANTONYM,
        FOR,
        // other
        IDENTIFIER,
    };

    static PARSING_TOKEN_TYPE GetParsingTokenType(ASTToken* token);

    std::shared_ptr<ErrorTable> error_table_;
    std::shared_ptr<PatternTable> arity_table_ =
      std::make_shared<PatternTable>();
};

} // namespace mizcore

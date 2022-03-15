#pragma once

#include <fstream>
#include <memory>

namespace mizcore {

class SymbolTable;
class TokenTable;
class MizFlexLexer;

class MizLexerHandler
{
  public:
    MizLexerHandler(std::istream* in,
                    const std::shared_ptr<SymbolTable>& symbol_table);
    virtual ~MizLexerHandler() = default;

    MizLexerHandler(const MizLexerHandler&) = delete;
    MizLexerHandler(MizLexerHandler&&) = delete;
    MizLexerHandler& operator=(const MizLexerHandler&) = delete;
    MizLexerHandler& operator=(MizLexerHandler&&) = delete;

    int yylex();
    std::shared_ptr<TokenTable> GetTokenTable() const;

    bool IsPartialMode() const { return is_partial_mode_; }
    void SetPartialMode(bool is_partial_mode)
    {
        is_partial_mode_ = is_partial_mode;
    }

  private:
    std::shared_ptr<MizFlexLexer> miz_flex_lexer_;
    bool is_partial_mode_ = false;
};

} // namespace mizcore

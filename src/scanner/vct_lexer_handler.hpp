#pragma once

#include <memory>

namespace mizcore {

class SymbolTable;
class VctFlexLexer;

class VctLexerHandler
{
  public:
    VctLexerHandler(std::istream* in);
    int yylex();

    std::shared_ptr<SymbolTable> GetSymbolTable() const;

  private:
    std::shared_ptr<VctFlexLexer> vct_flex_lexer_;
};

} // namespace mizcore
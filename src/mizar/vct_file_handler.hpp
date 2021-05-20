#pragma once

#include <memory>

namespace emcore::mizar {

class SymbolTable;
class VctFlexLexer;

class VctFileHandler
{
  public:
    VctFileHandler(std::istream* in);
    int yylex();

    std::shared_ptr<SymbolTable> GetSymbolTable() const;

  private:
    std::shared_ptr<VctFlexLexer> vct_flex_lexer_;
};

} // namespace emcore::mizar
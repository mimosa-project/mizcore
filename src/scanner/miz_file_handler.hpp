#pragma once

#include <fstream>
#include <memory>

namespace emcore {

class SymbolTable;
class TokenTable;
class MizFlexLexer;

class MizFileHandler
{
  public:
    MizFileHandler(std::istream* in, std::shared_ptr<SymbolTable> symbol_table);
    virtual ~MizFileHandler() = default;

    int yylex();
    std::shared_ptr<TokenTable> GetTokenTable() const;

  private:
    std::shared_ptr<MizFlexLexer> miz_flex_lexer_;
};

} // namespace emcore

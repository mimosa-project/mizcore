#pragma once

#include <fstream>
#include <memory>

#include "token_type.hpp"

namespace emcore::mizar {

class SymbolTable;
class TokenArray;
class MizFlexLexer;

class MizFileHandler {
public:
  MizFileHandler(std::istream *in, std::shared_ptr<SymbolTable> symbol_table);
  virtual ~MizFileHandler() = default;

  int yylex();
  std::shared_ptr<TokenArray> GetTokenArray() const;

private:
  std::shared_ptr<MizFlexLexer> miz_flex_lexer_;
};

} // namespace emcore::mizar

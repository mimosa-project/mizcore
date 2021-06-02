#pragma once

#include <memory>
#include <string>

namespace emcore {

class Symbol;
class SymbolTable;

class VctFlexLexer : public yyVctFlexLexer
{
  public:
    VctFlexLexer(std::istream* in);
    virtual int yylex();

    std::shared_ptr<SymbolTable> GetSymbolTable() const
    {
        return symbol_table_;
    }

  private:
    std::shared_ptr<SymbolTable> symbol_table_;
    std::string current_mizfile_;
};

} // namespace emcore

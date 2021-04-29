#pragma once

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace emcore::mizar {

class Symbol;
class SymbolTable;

class VctFileHandler : public yyVctFlexLexer {
public:
    VctFileHandler(std::istream *in);
    virtual int yylex();

    std::shared_ptr<SymbolTable> GetSymbolTable() const {return symbol_table_;}
private:
    std::shared_ptr<SymbolTable> symbol_table_;
    std::string current_mizfile_;
};

}
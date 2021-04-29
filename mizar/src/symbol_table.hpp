#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

namespace emcore::mizar {

class Symbol;

class SymbolTable {
public:
    SymbolTable() = default;
    virtual ~SymbolTable() = default;

    void AddSymbol(const std::string& filename, Symbol* symbol);
    void AddSynonym(Symbol* s0, Symbol* s1);
    std::vector<Symbol*> CollectFileSymbols(const std::string& filename) const;
    const std::vector<std::pair<Symbol*, Symbol*>>& CollectSynonyms() const;

private:
    std::map<std::string, std::vector<std::unique_ptr<Symbol>>> file2symbols_;
    std::vector<std::pair<Symbol*, Symbol*>> synonyms_;
    std::vector<Symbol*> valid_symbols_;
};

}

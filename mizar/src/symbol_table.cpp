#include "symbol.hpp"
#include "symbol_table.hpp"

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_pair;
using emcore::mizar::SYMBOL_TYPE;
using emcore::mizar::Symbol;
using emcore::mizar::SymbolTable;


void SymbolTable::AddSymbol(const string& filename, Symbol* symbol)
{
    if (file2symbols_.find(filename) == file2symbols_.end()) {
        file2symbols_[filename] = vector<unique_ptr<Symbol>>();
    }
    file2symbols_[filename].push_back(unique_ptr<Symbol>(symbol));
}

void SymbolTable::AddSynonym(Symbol* s0, Symbol* s1)
{
    synonyms_.push_back(make_pair(s0, s1));
}

std::vector<Symbol*> SymbolTable::CollectFileSymbols(const std::string& filename) const
{
    vector<Symbol*> symbols;
    auto it = file2symbols_.find(filename);
    if (it != file2symbols_.end()) {
        for (const auto& symbol : it->second) {
            symbols.push_back(symbol.get());
        }
    }
    return symbols;
}

const std::vector<std::pair<Symbol*, Symbol*>>& SymbolTable::CollectSynonyms() const
{
    return synonyms_;
}

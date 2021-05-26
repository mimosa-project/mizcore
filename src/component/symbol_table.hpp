#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "tsl/htrie_map.h"

namespace emcore {

class Symbol;

class SymbolTable
{
  public:
    // ctor, dtor
    SymbolTable();
    virtual ~SymbolTable() = default;

    // attributes
    void AddSymbol(std::string_view filename, Symbol* symbol);
    void AddSynonym(Symbol* s0, Symbol* s1);
    void AddValidFileName(std::string_view filename)
    {
        valid_filenames_.emplace_back(filename);
    }
    std::vector<Symbol*> CollectFileSymbols(std::string_view filename) const;
    const std::vector<std::pair<Symbol*, Symbol*>>& CollectSynonyms() const;

    // operations
    void Initialize();
    void BuildQueryMap();
    Symbol* QueryLongestMatchSymbol(std::string_view text) const;

  private:
    // implementation
    void BuildQueryMapOne(std::string_view filename);
    static bool IsWordBoundary(std::string_view text, size_t pos);
    static bool IsWordBoundaryCharacter(const char x);

    std::map<std::string, std::vector<std::unique_ptr<Symbol>>> file2symbols_;
    std::vector<std::pair<Symbol*, Symbol*>> synonyms_;
    std::vector<std::string> valid_filenames_;
    tsl::htrie_map<char, Symbol*> query_map_;
};

} // namespace emcore

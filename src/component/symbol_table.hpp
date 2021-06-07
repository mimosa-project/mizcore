#pragma once

#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "symbol_type.hpp"
#include "tsl/htrie_map.h"

namespace emcore {

class Symbol;

class SymbolTable
{
  public:
    // ctor, dtor
    SymbolTable();
    virtual ~SymbolTable() = default;

    SymbolTable(const SymbolTable&) = delete;
    SymbolTable(SymbolTable&&) = delete;
    SymbolTable& operator=(const SymbolTable&) = delete;
    SymbolTable& operator=(SymbolTable&&) = delete;

    // attributes
    Symbol* AddSymbol(std::string_view filename,
                      std::string_view text,
                      SYMBOL_TYPE type,
                      uint8_t priority = 64);
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
    static bool IsWordBoundaryCharacter(char x);

    std::map<std::string, std::vector<std::unique_ptr<Symbol>>> file2symbols_;
    std::vector<std::pair<Symbol*, Symbol*>> synonyms_;
    std::vector<std::string> valid_filenames_;
    tsl::htrie_map<char, Symbol*> query_map_;
};

} // namespace emcore

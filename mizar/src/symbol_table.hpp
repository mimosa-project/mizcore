#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "tsl/htrie_map.h"

namespace emcore::mizar {

class Symbol;

class SymbolTable {
public:
  // ctor, dtor
  SymbolTable();
  virtual ~SymbolTable() = default;

  // attributes
  void AddSymbol(const std::string &filename, Symbol *symbol);
  void AddSynonym(Symbol *s0, Symbol *s1);
  void AddValidFileName(const std::string &filename) {
    valid_filenames_.push_back(filename);
  }
  std::vector<Symbol *> CollectFileSymbols(const std::string &filename) const;
  const std::vector<std::pair<Symbol *, Symbol *>> &CollectSynonyms() const;

  // operations
  void Initialize();
  void BuildQueryMap();
  Symbol *QueryLongestMatchSymbol(const char *text) const;

private:
  // implementation
  void BuildQueryMapOne(const std::string &filename);
  static bool IsWordBoundary(const char *text, size_t pos);
  static bool IsWordBoundaryCharacter(const char x);

  std::map<std::string, std::vector<std::unique_ptr<Symbol>>> file2symbols_;
  std::vector<std::pair<Symbol *, Symbol *>> synonyms_;
  std::vector<std::string> valid_filenames_;
  tsl::htrie_map<char, Symbol *> query_map_;
};

} // namespace emcore::mizar

#include "symbol.hpp"
#include "symbol_table.hpp"

using std::string;
using std::vector;
using std::unique_ptr;
using std::make_pair;
using emcore::mizar::SYMBOL_TYPE;
using emcore::mizar::Symbol;
using emcore::mizar::SymbolTable;

SymbolTable::SymbolTable()
{
    Initialize();
}

void SymbolTable::Initialize()
{
    AddSymbol("SPECIAL_", new Symbol(",", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol(";", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol(":", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("(", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol(")", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("[", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("]", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("{", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("}", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("=", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("&", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("->", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol(".=", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("...", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$1", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$2", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$3", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$4", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$5", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$6", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$7", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$8", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$9", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("$10", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("(#", SYMBOL_TYPE::SPECIAL));
    AddSymbol("SPECIAL_", new Symbol("#)", SYMBOL_TYPE::SPECIAL));

    BuildQueryMapOne("SPECIAL_");
}

void SymbolTable::AddSymbol(const std::string& filename, Symbol* symbol)
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
        symbols.reserve(it->second.size());
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

void SymbolTable::BuildQueryMap()
{
    BuildQueryMapOne("HIDDEN");
    if (valid_filenames_.empty()) {
        for (const auto& pair : file2symbols_) {
            for (const auto& symbol_ptr : pair.second) {
                Symbol* symbol = symbol_ptr.get();
                query_map_[symbol->GetName()] = symbol;
            }
        }
    } else {
        for (const auto& filename : valid_filenames_) {
            BuildQueryMapOne(filename);
        }
    }
}

Symbol* SymbolTable::QueryLongestMatchSymbol(const char* text) const
{
    auto longest_prefix = query_map_.longest_prefix(text);
    if (longest_prefix != query_map_.end()) {
        if (IsWordBoundary(text, longest_prefix.key().length())) {
            return longest_prefix.value();
        }
    }
    return nullptr;
}

void SymbolTable::BuildQueryMapOne(const std::string& filename)
{
    auto it = file2symbols_.find(filename);
    if (it != file2symbols_.end()) {
        for (const auto& symbol_ptr : it->second) {
            Symbol* symbol = symbol_ptr.get();
            query_map_[symbol->GetName()] = symbol;
        }
    }
}

bool SymbolTable::IsWordBoundary(const char* text, size_t pos)
{
    size_t len = strlen(text);
    if (0 < pos && pos < len-1) {
        return  !IsWordBoundaryCharacter(text[pos-1])
             || !IsWordBoundaryCharacter(text[pos]);
    } else {
        return true;
    }
}

bool SymbolTable::IsWordBoundaryCharacter(const char x) {
    return isalnum(x) || (x == '_');
}
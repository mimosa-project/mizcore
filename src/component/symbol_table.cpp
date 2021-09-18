#include "symbol_table.hpp"
#include "symbol.hpp"

using std::string;
using std::unique_ptr;
using std::vector;

using mizcore::Symbol;
using mizcore::SYMBOL_TYPE;
using mizcore::SymbolTable;

SymbolTable::SymbolTable()
{
    Initialize();
}

void
SymbolTable::Initialize()
{
    query_map_is_built_ = false;

    AddSymbol("SPECIAL_", ",", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", ";", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", ":", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "(", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", ")", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "[", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "]", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "{", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "}", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "=", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "&", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "->", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", ".=", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "...", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$1", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$2", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$3", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$4", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$5", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$6", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$7", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$8", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$9", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "$10", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "(#", SYMBOL_TYPE::SPECIAL);
    AddSymbol("SPECIAL_", "#)", SYMBOL_TYPE::SPECIAL);

    BuildQueryMapOne("SPECIAL_");
}

Symbol*
SymbolTable::AddSymbol(std::string_view filename,
                       std::string_view text,
                       SYMBOL_TYPE type,
                       uint8_t priority)
{
    if (file2symbols_.find(string(filename)) == file2symbols_.end()) {
        file2symbols_[string(filename)] = vector<unique_ptr<Symbol>>();
    }
    auto p = std::make_unique<Symbol>(text, type, priority);
    Symbol* symbol = p.get();
    file2symbols_[string(filename)].emplace_back(std::move(p));
    return symbol;
}

void
SymbolTable::AddSynonym(Symbol* s0, Symbol* s1)
{
    synonyms_.emplace_back(s0, s1);
}

std::vector<Symbol*>
SymbolTable::CollectFileSymbols(std::string_view filename) const
{
    vector<Symbol*> symbols;
    auto it = file2symbols_.find(string(filename));
    if (it != file2symbols_.end()) {
        symbols.reserve(it->second.size());
        for (const auto& symbol : it->second) {
            symbols.push_back(symbol.get());
        }
    }
    return symbols;
}

const std::vector<std::pair<Symbol*, Symbol*>>&
SymbolTable::CollectSynonyms() const
{
    return synonyms_;
}

void
SymbolTable::BuildQueryMap()
{
    if (query_map_is_built_)
        return;

    BuildQueryMapOne("HIDDEN");
    if (valid_filenames_.empty()) {
        for (const auto& pair : file2symbols_) {
            for (const auto& symbol_ptr : pair.second) {
                Symbol* symbol = symbol_ptr.get();
                query_map_[symbol->GetText()] = symbol;
            }
        }
    } else {
        for (const auto& filename : valid_filenames_) {
            BuildQueryMapOne(filename);
        }
    }
    query_map_is_built_ = true;
}

Symbol*
SymbolTable::QueryLongestMatchSymbol(std::string_view text) const
{
    auto longest_prefix = query_map_.longest_prefix(text);
    if (longest_prefix != query_map_.end()) {
        if (IsWordBoundary(text, longest_prefix.key().length())) {
            return longest_prefix.value();
        }
    }
    return nullptr;
}

void
SymbolTable::BuildQueryMapOne(std::string_view filename)
{
    auto it = file2symbols_.find(string(filename));
    if (it != file2symbols_.end()) {
        for (const auto& symbol_ptr : it->second) {
            Symbol* symbol = symbol_ptr.get();
            query_map_[symbol->GetText()] = symbol;
        }
    }
}

bool
SymbolTable::IsWordBoundary(std::string_view text, size_t pos)
{
    if (0 < pos && pos < text.size() - 1) {
        return !IsWordBoundaryCharacter(text[pos - 1]) ||
               !IsWordBoundaryCharacter(text[pos]);
    }
    return true;
}

bool
SymbolTable::IsWordBoundaryCharacter(const char x)
{
    return static_cast<bool>(isalnum(x)) || (x == '_');
}
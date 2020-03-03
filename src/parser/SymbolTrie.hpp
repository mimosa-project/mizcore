#pragma once

#include <cstdint>
#include <utility>
#include <set>
#include <string>
#include <array>
#include <tsl/htrie_map.h>

namespace Mimosa {

class SymbolTrie {
public:
    // ctor,dtor
    SymbolTrie() {}
    virtual ~SymbolTrie() {}

    // accessor
    void RegisterArticle(const std::string& article);
    bool IsRegisteredArticle(const std::string& article) const;

    void InsertSymbol(const std::string& key,
                      const std::array<uint8_t,2>& value);
    void EraseSymbol(const std::string& key);
    
    std::pair<std::string, std::array<uint8_t,2>>
    FindLongestMatch(const char* text) const;
    
    std::pair<std::string, std::array<uint8_t,2>>
    FindLongestMatchWithKeySize(const char* text, size_t key_size) const;
    
    size_t GetRegisteredSymbolNum() const {return m_trie_map.size();}

    // operator
    void Initialize();
    bool BuildSymbolTrie(const char* mmlVctPath);
    void SaveTrieMap(const char* path);
    void LoadTrieMap(const char* path);

private:
    std::set<std::string> m_articles;
    tsl::htrie_map<char, std::array<uint8_t,2>> m_trie_map;
};

}
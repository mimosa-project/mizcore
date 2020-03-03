#include "SymbolTrie.hpp"
#include <cassert>
#include <fstream>
#include <type_traits>
#include <spdlog/spdlog.h>

using namespace std;
using namespace Mimosa;

#define IS_LITTLE_ENDIAN (1 == *(unsigned char *)&(const int){1})

namespace {
    string trim_right(const string& str) {
        size_t right = str.find_last_not_of(" \t\v\r\n");
        return str.substr(0, right+1);
    }

    string to_upper(const string& str) {
        string tmp = str;
        transform(tmp.begin(), tmp.end(), tmp.begin(), ::toupper);
        return tmp;
    }

    template <typename T>
    T SwapEndian(const T &val) {
        union U {
            T val;
            std::array<std::uint8_t, sizeof(T)> raw;
        } src, dst;

        src.val = val;
        std::reverse_copy(src.raw.begin(), src.raw.end(), dst.raw.begin());
        return dst.val;
    }

    class Serializer {
    public:
        Serializer(const char* file_name) {
            m_ostream.exceptions(m_ostream.badbit | m_ostream.failbit);
            m_ostream.open(file_name);
        }
        
        template<typename T>
        void operator()(const T& value) {
#ifdef IS_LITTLE_ENDIAN
            T swapped = SwapEndian(value);
            m_ostream.write(reinterpret_cast<const char*>(&swapped), sizeof(T));
#else
            m_ostream.write(reinterpret_cast<const char*>(&value), sizeof(T));
#endif
        }

        void operator()(const char* value, std::size_t value_size) {
            m_ostream.write(value, value_size);
        }

    private:
        std::ofstream m_ostream;
    };

    class Deserializer {
    public:
        Deserializer(const char* file_name) {
            m_istream.exceptions(m_istream.badbit | m_istream.failbit | m_istream.eofbit);
            m_istream.open(file_name);
        }
        
        template<class T>
        T operator()() {
            T value;
            m_istream.read(reinterpret_cast<char*>(&value), sizeof(T));
#ifdef IS_LITTLE_ENDIAN
            return SwapEndian(value);
#else
            return value;
#endif
        }
        
        void operator()(char* value, std::size_t value_size) {
            m_istream.read(value, value_size);
        }

    private:
        std::ifstream m_istream;
    };

}

/// @brief Specify articles to be loaded in mml.vct
/// @param article  article name
void SymbolTrie::RegisterArticle(const std::string& article) {
    string uppercase = to_upper(article);
    m_articles.insert(uppercase);
}

/// @brief Check if article is set
/// @param article  article name
/// @return article is set or not
bool SymbolTrie::IsRegisteredArticle(const std::string& article) const {
    string uppercase = to_upper(article);
    return m_articles.end() != m_articles.find(uppercase);
}


/// @brief Insert symbol to trie map
/// @param key   key string
/// @param value symbol type and priority
void SymbolTrie::InsertSymbol(const std::string& key, const std::array<uint8_t,2>& value) {
    m_trie_map[key] = value;
}

/// @brief Erase symbol in trie map
/// @param key   key string
void SymbolTrie::EraseSymbol(const std::string& key) {
    m_trie_map.erase(key);
}

/// @brief Find longest match key in trie map
/// @param text text string to check match
/// @return if longest match key is found, return its key length and value
///         otherwire, return zero as key length.
pair<string, array<uint8_t,2>>
SymbolTrie::FindLongestMatch(const char* text) const {
    auto longest_prefix = m_trie_map.longest_prefix(text);
    if (longest_prefix != m_trie_map.end()) {
        auto key = longest_prefix.key();
        auto value = longest_prefix.value();
        return make_pair(key, value);
    } else {
        return make_pair(string(), array<uint8_t,2>{0,0});
    }
}

/// @brief Find longest match key (less than key_size characters) in trie map
/// @param text text string to check match
/// @param key_size key size
/// @return if longest match key is found, return its key and value
///         otherwire, return empty string and zero array.
pair<string, array<uint8_t,2>>
SymbolTrie::FindLongestMatchWithKeySize(const char* text, size_t key_size) const {
    auto longest_prefix = m_trie_map.longest_prefix_ks(text, key_size);
    if (longest_prefix != m_trie_map.end()) {
        auto key = longest_prefix.key();
        auto value = longest_prefix.value();
        return make_pair(key, value);
    } else {
        return make_pair(string(), array<uint8_t,2>{0,0});
    }
}

/// @brief Initialize class
void SymbolTrie::Initialize() {
    m_articles.clear();
    m_trie_map.clear();
}

/// @brief Build symbol trie by loading mml.vct
/// @return Success (true) or not (false)
/// 
/// Only symbols declared in articles appeared in m_articles. 
/// If m_articles are not set, this function insert all symbols into trie.
/// The format of mml.vct is as follows:
///   #ALGSEQ_1                 <-- file name
///   G0 K0 L0 M1 O0 R1 U1 V1   <-- number of each type of symbols
///   Vfinite-Support           <-- type and symbol 
///   MAlgSequence
///   Ris_at_least_length_of
///   Oleq 32                   <-- functor may have priority
///   #ALGSPEC1
///   G0 K0 L0 M2 O1 R2 U0 V0 
///   O-indexing
///   Mrng-retract
///   Rform_a_replacement_in
///   MExtension
///   Rrun runs                 <-- some symbol may have two expression
bool SymbolTrie::BuildSymbolTrie(const char* mmlVctPath) {
    assert(mmlVctPath);

    if (!m_articles.empty()) {
        m_articles.insert("HIDDEN");
    }

    ifstream ifs(mmlVctPath);
    if (!ifs) {
        spdlog::error("Failed to open mml.vct : path = {}", mmlVctPath);
        return false;
    }

    string line;
    bool skip_a_line = false;
    bool is_parsing = false;
    while (getline(ifs, line)) {
        if (skip_a_line) {
            skip_a_line = false;
            continue;
        }

        line = trim_right(line);
        if (line[0] == '#') {
            skip_a_line = true;
            if (m_articles.empty()) {
                is_parsing = true;
            } else {
                is_parsing = IsRegisteredArticle(line.substr(1));
            }
            continue;
        }

        if (is_parsing) {
            uint8_t symbol_type = (uint8_t)line[0];
            uint8_t default_priority = 64;
            size_t space_pos = line.find_first_of(' ');
            if (string::npos == space_pos) {
                // only one symbol is defined
                string key = line.substr(1);
                array<uint8_t, 2> value = {symbol_type, default_priority};
                m_trie_map[key] = value;
            } else {
                assert(string::npos == line.find_first_of(' ', space_pos+1));
                if (symbol_type == 'O') {
                    // priority is included
                    string key = line.substr(1, space_pos-1);
                    uint8_t priority = (uint8_t)stoi(line.substr(space_pos+1));
                    array<uint8_t, 2> value = {symbol_type, priority};
                    m_trie_map[key] = value;
                } else {
                    // two names are defined
                    string key1 = line.substr(1, space_pos-1);
                    string key2 = line.substr(space_pos+1);
                    array<uint8_t, 2> value = {symbol_type, default_priority};
                    m_trie_map[key1] = value;
                    m_trie_map[key2] = value;
                }
            }
        }
    }
    
    return true;
}

/// @brief Save trie data to file
/// @param path  file path to save data
void SymbolTrie::SaveTrieMap(const char* path) {
    Serializer serializer(path);
    m_trie_map.serialize(serializer);
}

/// @brief Load trie data from file
/// @param path  file path to load data
void SymbolTrie::LoadTrieMap(const char* path) {
    Deserializer dserializer(path);
    m_trie_map = tsl::htrie_map<char, array<uint8_t,2>>::deserialize(dserializer);
}


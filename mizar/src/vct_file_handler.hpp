#pragma once

#include <utility>
#include <string>
#include <map>
#include <vector>
#include "symbol.hpp"

namespace emcore::mizar {

class VctFileHandler : public yyVctFlexLexer {
public:
    VctFileHandler(std::istream *in) : yyVctFlexLexer(in) {}
    virtual int yylex();

    std::vector<Symbol> GetFileSymbols(const std::string& filename) const {
        auto it = file2symbols_.find(filename);
        if (it != file2symbols_.end()) {
            return it->second;
        } else {
            return std::vector<Symbol>();
        }
    }
    
    const std::vector<std::pair<std::string, std::string>>& GetSynonyms() const {
        return synonyms_;
    }
private:
    std::map<std::string, std::vector<Symbol>> file2symbols_;
    std::vector<std::pair<std::string, std::string>> synonyms_;
    std::string current_mizfile_;
};

}
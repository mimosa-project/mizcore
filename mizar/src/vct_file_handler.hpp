#pragma once

#include <utility>
#include <string>
#include <map>
#include <vector>
#include <memory>

namespace emcore::mizar {

class Symbol;

class VctFileHandler : public yyVctFlexLexer {
public:
    using SharedPtr = std::shared_ptr<Symbol>;

    VctFileHandler(std::istream *in) : yyVctFlexLexer(in) {}
    virtual int yylex();

    std::vector<SharedPtr> GetFileSymbols(const std::string& filename) const {
        auto it = file2symbols_.find(filename);
        if (it != file2symbols_.end()) {
            return it->second;
        } else {
            return std::vector<SharedPtr>();
        }
    }
    
    const std::vector<std::pair<SharedPtr, SharedPtr>>& GetSynonyms() const {
        return synonyms_;
    }
private:
    std::map<std::string, std::vector<SharedPtr>> file2symbols_;
    std::vector<std::pair<SharedPtr, SharedPtr>> synonyms_;
    std::string current_mizfile_;
};

}
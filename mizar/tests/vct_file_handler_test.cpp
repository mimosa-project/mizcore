#include "doctest.h"
#include <iostream>
#include <fstream>
#include <filesystem>

#undef yyFlexLexer
#define yyFlexLexer yyVctFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer
#include "vct_file_handler.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using emcore::mizar::SYMBOL_TYPE;
using emcore::mizar::Symbol;
using emcore::mizar::VctFileHandler;
namespace fs = std::filesystem;

static fs::path TEST_DATA_DIR = fs::path(__FILE__).parent_path() / "data";
TEST_CASE("execute vct scanner") {
    fs::path mml_vct_path = TEST_DATA_DIR / "mml.vct";
    ifstream ifs(mml_vct_path.c_str());

    // Input file existence
    CHECK(ifs.good());

    VctFileHandler handler(&ifs);
    handler.yylex();
    
    auto symbols = handler.GetFileSymbols("GROUP_1");
    CHECK(symbols.size() == 9);

    CHECK(symbols[0].GetName() == "unital");
    CHECK(symbols[0].GetPriority() == 64);
    CHECK(symbols[0].GetType() == SYMBOL_TYPE::ATTRIBUTE);

    CHECK(symbols[4].GetName() == "inverse_op");
    CHECK(symbols[4].GetPriority() == 124);
    CHECK(symbols[4].GetType() == SYMBOL_TYPE::FUNCTOR);

    symbols = handler.GetFileSymbols("SCMFSA7B");
    CHECK(symbols.size() == 6);
    
    CHECK(symbols[0].GetName() == "refers");
    CHECK(symbols[0].GetPriority() == 64);
    CHECK(symbols[0].GetType() == SYMBOL_TYPE::PREDICATE);

    CHECK(symbols[1].GetName() == "refer");
    CHECK(symbols[1].GetPriority() == 64);
    CHECK(symbols[1].GetType() == SYMBOL_TYPE::PREDICATE);
    
    auto synonyms = handler.GetSynonyms();
    CHECK(synonyms.size() == 5);
}

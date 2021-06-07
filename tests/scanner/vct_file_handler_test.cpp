#include <filesystem>
#include <fstream>
#include <iostream>

#include "doctest/doctest.h"
#include "spdlog/cfg/env.h"
#include "spdlog/spdlog.h"

#undef yyFlexLexer
#define yyFlexLexer yyVctFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer

#include "symbol.hpp"
#include "symbol_table.hpp"
#include "vct_file_handler.hpp"

using emcore::SYMBOL_TYPE;
using emcore::SymbolTable;
using emcore::VctFileHandler;
using std::ifstream;
namespace fs = std::filesystem;

const fs::path TEST_DATA_DIR = fs::path(__FILE__).parent_path() / "data";

TEST_CASE("execute vct scanner")
{
    fs::path mml_vct_path = TEST_DATA_DIR / "mml.vct";
    ifstream ifs(mml_vct_path.c_str());

    // Input file existence
    CHECK(ifs.good());

    VctFileHandler handler(&ifs);
    handler.yylex();

    std::shared_ptr<SymbolTable> table = handler.GetSymbolTable();
    auto symbols = table->CollectFileSymbols("GROUP_1");
    CHECK(symbols.size() == 9);

    CHECK(symbols[0]->GetText() == "unital");
    CHECK(symbols[0]->GetPriority() == 64);
    CHECK(symbols[0]->GetType() == SYMBOL_TYPE::ATTRIBUTE);

    CHECK(symbols[4]->GetText() == "inverse_op");
    CHECK(symbols[4]->GetPriority() == 124);
    CHECK(symbols[4]->GetType() == SYMBOL_TYPE::FUNCTOR);

    symbols = table->CollectFileSymbols("SCMFSA7B");
    CHECK(symbols.size() == 6);

    CHECK(symbols[0]->GetText() == "refers");
    CHECK(symbols[0]->GetPriority() == 64);
    CHECK(symbols[0]->GetType() == SYMBOL_TYPE::PREDICATE);

    CHECK(symbols[1]->GetText() == "refer");
    CHECK(symbols[1]->GetPriority() == 64);
    CHECK(symbols[1]->GetType() == SYMBOL_TYPE::PREDICATE);

    auto synonyms = table->CollectSynonyms();
    CHECK(synonyms.size() == 5);
}

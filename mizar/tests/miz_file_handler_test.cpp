#include "doctest/doctest.h"
#include <iostream>
#include <fstream>
#include <filesystem>

#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token_array.hpp"
#include "vct_file_handler.hpp"
#include "miz_file_handler.hpp"

using std::cout;
using std::endl;
using std::ifstream;
using emcore::mizar::SYMBOL_TYPE;
using emcore::mizar::Symbol;
using emcore::mizar::SymbolTable;
using emcore::mizar::VctFileHandler;
using emcore::mizar::MizFileHandler;
namespace fs = std::filesystem;

static fs::path TEST_DATA_DIR = fs::path(__FILE__).parent_path() / "data";
TEST_CASE("execute miz file handler") {
    std::shared_ptr<SymbolTable> symbol_table;
    {
        fs::path mml_vct_path = TEST_DATA_DIR / "mml.vct";
        ifstream ifs(mml_vct_path.c_str());

        // Input file existence
        CHECK(ifs.good());

        VctFileHandler handler(&ifs);
        handler.yylex();
        symbol_table = handler.GetSymbolTable();
    }

    SUBCASE("NUMERALS.miz") {
        fs::path miz_file_path = TEST_DATA_DIR / "numerals.miz";
        ifstream ifs(miz_file_path.c_str());
        MizFileHandler miz_handler(&ifs, symbol_table);
        miz_handler.yylex();

        auto token_array = miz_handler.GetTokenArray();
        cout << "Token Num: " << token_array->GetTokenNum() << endl;
        token_array->Dump(cout);
        cout << endl;
    }

    /*
    SUBCASE("jgraph_4.miz") {
        fs::path miz_file_path = TEST_DATA_DIR / "jgraph_4.miz";
        ifstream ifs(miz_file_path.c_str());
        MizFileHandler miz_handler(&ifs, symbol_table);
        miz_handler.yylex();

        auto token_array = miz_handler.GetTokenArray();
        cout << "Token Num: " << token_array->GetTokenNum() << endl;
        // token_array->Dump(cout);
        // cout << endl;
    }
    */
}

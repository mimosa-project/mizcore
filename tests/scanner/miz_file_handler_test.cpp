#include <filesystem>
#include <fstream>
#include <iostream>

#include "doctest/doctest.h"
#include "miz_file_handler.hpp"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token.hpp"
#include "token_table.hpp"
#include "vct_file_handler.hpp"

using emcore::MizFileHandler;
using emcore::SymbolTable;
using emcore::VctFileHandler;
using std::string;
namespace fs = std::filesystem;

static bool
check_text_diff(const fs::path& path1, const fs::path& path2)
{
    bool diff_found = false;

    std::ifstream ifs1(path1);
    std::ifstream ifs2(path2);

    string line1;
    string line2;
    for (size_t i = 1;; ++i) {
        line1.clear();
        line2.clear();
        std::getline(ifs1, line1);
        std::getline(ifs2, line2);
        if (line1.empty() && line2.empty()) {
            break;
        }

        if (line1 != line2) {
            diff_found = true;
            std::cout << "Diff found: line = " << i << ":\n"
                      << line1 << "\n"
                      << line2 << std::endl;
        }
    }

    return diff_found;
}

const fs::path TEST_DIR = fs::path(__FILE__).parent_path();

TEST_CASE("execute miz file handler")
{
    std::shared_ptr<SymbolTable> symbol_table;
    {
        fs::path mml_vct_path = TEST_DIR / "data" / "mml.vct";
        std::ifstream ifs(mml_vct_path);

        // Input file existence
        CHECK(ifs.good());

        VctFileHandler vct_handler(&ifs);

        // Erapsed time: 0.057 [s]
        // clock_t start = clock();
        vct_handler.yylex();
        // clock_t duration = clock() - start;
        // std::cout << "The elapsed time [s] of VctFileHandler is: "
        //           << (double)duration / CLOCKS_PER_SEC << std::endl;
        symbol_table = vct_handler.GetSymbolTable();
    }

    SUBCASE("NUMERALS.miz")
    {
        fs::path miz_file_path = TEST_DIR / "data" / "numerals.miz";
        std::ifstream ifs(miz_file_path);
        MizFileHandler miz_handler(&ifs, symbol_table);

        // Erapsed time: 0.00034 [s]
        // clock_t start = clock();
        miz_handler.yylex();
        // clock_t duration = clock() - start;
        // std::cout << "The elapsed time [s] of VctFileHandler is: "
        //           << (double)duration / CLOCKS_PER_SEC << std::endl;

        auto token_array = miz_handler.GetTokenTable();
        CHECK(79 == token_array->GetTokenNum());

        if (!fs::exists(TEST_DIR / "result")) {
            fs::create_directory(TEST_DIR / "result");
        }

        fs::path result_file_path = TEST_DIR / "result" / "numerals_tokens.txt";
        {
            std::ofstream ofs(result_file_path);
            for (size_t i = 0; i < token_array->GetTokenNum(); ++i) {
                ofs << i << ": " << token_array->GetToken(i)->ToJson().dump()
                    << std::endl;
            }
            ofs << std::endl;
        }

        fs::path expected_file_path =
          TEST_DIR / "expected" / "numerals_tokens.txt";
        CHECK(!check_text_diff(result_file_path, expected_file_path));
        remove(result_file_path.c_str());
    }

    /*
    SUBCASE("jgraph_4.miz")
    {
        fs::path miz_file_path = TEST_DIR / "data" / "jgraph_4.miz";
        std::ifstream ifs(miz_file_path.c_str());
        MizFileHandler miz_handler(&ifs, symbol_table);

        // Erapsed time: 0.221 [ms]
        // clock_t start = clock();
        miz_handler.yylex();
        // clock_t duration = clock() - start;
        // std::cout << "The elapsed time [s] of MizFileHandler is: "
        //           << (double)duration / CLOCKS_PER_SEC << std::endl;

        auto token_array = miz_handler.GetTokenTable();
        CHECK(186748 == token_array->GetTokenNum());

        // std::cout << "Token Num: " << token_array->GetTokenNum() <<
        // std::endl;
        // token_array->Dump(std::cout);
        // std::cout << std::endl;
    }
    */
}

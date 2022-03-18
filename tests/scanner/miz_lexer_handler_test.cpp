#include <filesystem>
#include <fstream>
#include <iostream>

#include "ast_token.hpp"
#include "doctest/doctest.h"
#include "file_handling_tools.hpp"
#include "miz_lexer_handler.hpp"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token_table.hpp"
#include "vct_lexer_handler.hpp"

using mizcore::MizLexerHandler;
using mizcore::SymbolTable;
using mizcore::VctLexerHandler;
using std::string;
namespace fs = std::filesystem;

namespace {

const fs::path&
TEST_DIR()
{
    static fs::path test_dir = fs::path(__FILE__).parent_path();
    return test_dir;
}

} // namespace

TEST_CASE("execute miz file handler")
{
    std::shared_ptr<SymbolTable> symbol_table;
    {
        fs::path mml_vct_path = TEST_DIR() / "data" / "mml.vct";
        std::ifstream ifs(mml_vct_path);

        // Input file existence
        CHECK(ifs.good());

        VctLexerHandler vct_handler(&ifs);

        // Erapsed time: 0.057 [s]
        // clock_t start = clock();
        vct_handler.yylex();
        // clock_t duration = clock() - start;
        // std::cout << "The elapsed time [s] of VctLexerHandler is: "
        //           << (double)duration / CLOCKS_PER_SEC << std::endl;
        symbol_table = vct_handler.GetSymbolTable();
    }

    SUBCASE("NUMERALS.miz")
    {
        fs::path miz_file_path = TEST_DIR() / "data" / "numerals.miz";
        std::ifstream ifs(miz_file_path);
        MizLexerHandler miz_handler(&ifs, symbol_table);

        // Erapsed time: 0.00034 [s]
        clock_t start = clock();
        miz_handler.yylex();
        clock_t duration = clock() - start;
        std::cout
          << "The elapsed time [s] of MizLexerHandler for NUMERALS.miz is: "
          << static_cast<double>(duration) / CLOCKS_PER_SEC << std::endl;

        auto token_table = miz_handler.GetTokenTable();
        CHECK(79 == token_table->GetTokenNum());

        if (!fs::exists(TEST_DIR() / "result")) {
            fs::create_directory(TEST_DIR() / "result");
        }

        fs::path result_file_path =
          TEST_DIR() / "result" / "numerals_tokens.json";
        {
            nlohmann::json json;
            token_table->ToJson(json);
            mizcore::write_json_file(json, result_file_path);
        }

        fs::path expected_file_path =
          TEST_DIR() / "expected" / "numerals_tokens.json";

        auto json_diff =
          mizcore::json_file_diff(result_file_path, expected_file_path);
        CHECK(json_diff.empty());

        if (!json_diff.empty()) {
            fs::path diff_file_path =
              TEST_DIR() / "result" / "numerals_tokens_diff.json";
            mizcore::write_json_file(json_diff, diff_file_path);
        } else {
            remove(result_file_path.c_str());
        }
    }

    SUBCASE("jgraph_4.miz")
    {
        fs::path miz_file_path = TEST_DIR() / "data" / "jgraph_4.miz";
        std::ifstream ifs(miz_file_path);
        MizLexerHandler miz_handler(&ifs, symbol_table);

        // Erapsed time: 1.05308 [s]
        clock_t start = clock();
        miz_handler.yylex();
        clock_t duration = clock() - start;
        std::cout
          << "The elapsed time [s] of MizLexerHandler for jgraph_4.miz is: "
          << static_cast<double>(duration) / CLOCKS_PER_SEC << std::endl;

        auto token_table = miz_handler.GetTokenTable();
        CHECK(186748 == token_table->GetTokenNum());

        if (!fs::exists(TEST_DIR() / "result")) {
            fs::create_directory(TEST_DIR() / "result");
        }

        fs::path result_file_path =
          TEST_DIR() / "result" / "jgraph_4_tokens.json";
        {
            nlohmann::json json;
            token_table->ToJson(json);
            mizcore::write_json_file(json, result_file_path);
        }

        fs::path expected_file_path =
          TEST_DIR() / "expected" / "jgraph_4_tokens.json";

        auto json_diff =
          mizcore::json_file_diff(result_file_path, expected_file_path);
        CHECK(json_diff.empty());

        if (!json_diff.empty()) {
            fs::path diff_file_path =
              TEST_DIR() / "result" / "jgraph_4_tokens_diff.json";
            mizcore::write_json_file(json_diff, diff_file_path);
        } else {
            remove(result_file_path.c_str());
        }
    }
}

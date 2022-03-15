#include <filesystem>
#include <fstream>
#include <iostream>

#include "ast_block.hpp"
#include "doctest/doctest.h"
#include "file_handling_tools.hpp"
#include "miz_block_parser.hpp"
#include "miz_lexer_handler.hpp"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token.hpp"
#include "token_table.hpp"
#include "vct_lexer_handler.hpp"

using mizcore::MizBlockParser;
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
        vct_handler.yylex();
        symbol_table = vct_handler.GetSymbolTable();
    }

    SUBCASE("NUMERALS.miz")
    {
        fs::path miz_file_path = TEST_DIR() / "data" / "numerals.miz";
        std::ifstream ifs(miz_file_path);
        MizLexerHandler miz_handler(&ifs, symbol_table);
        miz_handler.yylex();
        auto token_table = miz_handler.GetTokenTable();

        MizBlockParser miz_block_parser(token_table);

        // Erapsed time: 0.000168 [s]
        clock_t start = clock();
        miz_block_parser.Parse();
        clock_t duration = clock() - start;
        std::cout
          << "The elapsed time [s] of MizBlockParser for NUMERALS.miz is: "
          << static_cast<double>(duration) / CLOCKS_PER_SEC << std::endl;

        auto ast_root = miz_block_parser.GetASTRoot();

        if (!fs::exists(TEST_DIR() / "result")) {
            fs::create_directory(TEST_DIR() / "result");
        }

        fs::path result_file_path =
          TEST_DIR() / "result" / "numerals_blocks.json";
        {
            nlohmann::json json;
            ast_root->ToJson(json);
            mizcore::write_json_file(json, result_file_path);
        }

        fs::path expected_file_path =
          TEST_DIR() / "expected" / "numerals_blocks.json";

        auto json_diff =
          mizcore::json_file_diff(result_file_path, expected_file_path);

        CHECK(json_diff.empty());
        if (!json_diff.empty()) {
            fs::path diff_file_path =
              TEST_DIR() / "result" / "numerals_blocks_diff.json";
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
        miz_handler.yylex();
        auto token_table = miz_handler.GetTokenTable();

        MizBlockParser miz_block_parser(token_table);

        // Erapsed time: 0.021594 [s]
        clock_t start = clock();
        miz_block_parser.Parse();
        clock_t duration = clock() - start;
        std::cout
          << "The elapsed time [s] of MizBlockParser for jgraph_4.miz is: "
          << static_cast<double>(duration) / CLOCKS_PER_SEC << std::endl;

        auto ast_root = miz_block_parser.GetASTRoot();

        if (!fs::exists(TEST_DIR() / "result")) {
            fs::create_directory(TEST_DIR() / "result");
        }

        fs::path result_file_path =
          TEST_DIR() / "result" / "jgraph_4_blocks.json";
        {
            nlohmann::json json;
            ast_root->ToJson(json);
            mizcore::write_json_file(json, result_file_path);
        }

        fs::path expected_file_path =
          TEST_DIR() / "expected" / "jgraph_4_blocks.json";

        auto json_diff =
          mizcore::json_file_diff(result_file_path, expected_file_path);

        CHECK(json_diff.empty());
        if (!json_diff.empty()) {
            fs::path diff_file_path =
              TEST_DIR() / "result" / "jgraph_4_blocks_diff.json";
            mizcore::write_json_file(json_diff, diff_file_path);
        } else {
            remove(result_file_path.c_str());
        }
    }
}

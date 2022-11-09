#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>

#include "ast_block.hpp"
#include "ast_token.hpp"
#include "doctest/doctest.h"
#include "error_table.hpp"
#include "file_handling_tools.hpp"
#include "miz_block_parser.hpp"
#include "miz_lexer_handler.hpp"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token_table.hpp"
#include "vct_lexer_handler.hpp"

using mizcore::ErrorTable;
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

void
check_parser_one(const char* article_name,
                 std::shared_ptr<SymbolTable>& symbol_table)
{
    fs::path miz_file_path =
      TEST_DIR() / "data" / (std::string(article_name) + ".miz");

    fs::path result_dir = TEST_DIR() / "result";
    fs::path expected_dir = TEST_DIR() / "expected";

    std::ifstream ifs(miz_file_path);
    MizLexerHandler miz_handler(&ifs, symbol_table);
    miz_handler.yylex();
    auto token_table = miz_handler.GetTokenTable();
    auto error_table = std::make_shared<ErrorTable>();

    MizBlockParser miz_block_parser(token_table, error_table);

    // Erapsed time: 0.000168 [s]
    clock_t start = clock();
    miz_block_parser.Parse();
    clock_t duration = clock() - start;

    std::ostringstream oss;
    oss << "The elapsed time [s] of MizBlockParser for " << article_name
        << " is: " << static_cast<double>(duration) / CLOCKS_PER_SEC
        << std::endl;
    INFO(oss.str());
    std::cerr << oss.str();

    if (!fs::exists(result_dir)) {
        fs::create_directory(result_dir);
    }

    // token diff
    fs::path result_token_path =
      result_dir / (std::string(article_name) + "_tokens.json");
    {
        nlohmann::json json;
        token_table->ToJson(json);
        mizcore::write_json_file(json, result_token_path);
    }

    fs::path expected_token_path =
      expected_dir / (std::string(article_name) + "_tokens.json");

    auto token_diff_json =
      mizcore::json_file_diff(result_token_path, expected_token_path);

    CHECK(token_diff_json.empty());
    if (!token_diff_json.empty()) {
        fs::path token_diff_path =
          result_dir / (std::string(article_name) + "_tokens_diff.json");
        mizcore::write_json_file(token_diff_json, token_diff_path);
    } else {
        remove(result_token_path.string().c_str());
    }

    // block diff
    auto ast_root = miz_block_parser.GetASTRoot();

    fs::path result_block_path =
      result_dir / (std::string(article_name) + "_blocks.json");
    {
        nlohmann::json json;
        ast_root->ToJson(json);
        mizcore::write_json_file(json, result_block_path);
    }

    fs::path expected_block_path =
      expected_dir / (std::string(article_name) + "_blocks.json");

    auto block_diff_json =
      mizcore::json_file_diff(result_block_path, expected_block_path);

    CHECK(block_diff_json.empty());
    if (!block_diff_json.empty()) {
        fs::path block_diff_path =
          result_dir / (std::string(article_name) + "_blocks_diff.json");
        mizcore::write_json_file(block_diff_json, block_diff_path);
    } else {
        remove(result_block_path.string().c_str());
    }
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

    SUBCASE("NUMERALS.miz") { check_parser_one("numerals", symbol_table); }

    SUBCASE("AXIOMS_MOD_NG.miz")
    {
        check_parser_one("axioms_mod_ng", symbol_table);
    }

    SUBCASE("jgraph_4.miz") { check_parser_one("jgraph_4", symbol_table); }
}

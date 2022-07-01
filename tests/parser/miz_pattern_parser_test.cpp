#include <filesystem>
#include <fstream>
#include <iostream>
#include <locale>
#include <memory>

#include "ast_block.hpp"
#include "ast_token.hpp"
#include "doctest/doctest.h"
#include "error_table.hpp"
#include "file_handling_tools.hpp"
#include "miz_block_parser.hpp"
#include "miz_lexer_handler.hpp"
#include "miz_pattern_parser.hpp"
#include "pattern_table.hpp"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token_table.hpp"
#include "vct_lexer_handler.hpp"

using mizcore::ErrorTable;
using mizcore::MizBlockParser;
using mizcore::MizLexerHandler;
using mizcore::MizPatternParser;
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

std::string
uppercase_of(const std::string& s)
{
    std::locale loc;
    std::string us;
    for (char c : s) {
        us += std::toupper(c, loc);
    }
    return us;
}

void
TestPatternParser(const std::string& name,
                  const std::shared_ptr<SymbolTable>& symbol_table)
{
    fs::path miz_file_path = TEST_DIR() / "data" / (name + ".miz");
    std::ifstream ifs(miz_file_path);
    MizLexerHandler miz_handler(&ifs, symbol_table);
    miz_handler.yylex();
    auto token_table = miz_handler.GetTokenTable();
    auto error_table = std::make_shared<ErrorTable>();

    MizBlockParser miz_block_parser(token_table, error_table);
    miz_block_parser.Parse();
    auto ast_root = miz_block_parser.GetASTRoot();

    MizPatternParser miz_pattern_parser(error_table);

    // Erapsed time: ?.???? [s]
    clock_t start = clock();
    miz_pattern_parser.ParseBlock(
      ast_root.get(), token_table, uppercase_of(name));
    clock_t duration = clock() - start;
    std::cout << std::string("The elapsed time [s] of MizPatternParser for ") +
                   name + ".miz is: "
              << static_cast<double>(duration) / CLOCKS_PER_SEC << std::endl;
    auto pattern_table = miz_pattern_parser.GetArityTable();

    if (!fs::exists(TEST_DIR() / "result")) {
        fs::create_directory(TEST_DIR() / "result");
    }

    fs::path result_file_path =
      TEST_DIR() / "result" / (name + "_pattern_table.json");
    {
        nlohmann::json json;
        pattern_table->ToJson(json);
        mizcore::write_json_file(json, result_file_path);
    }

    fs::path expected_file_path =
      TEST_DIR() / "expected" / (name + "_pattern_table.json");

    auto json_diff =
      mizcore::json_file_diff(result_file_path, expected_file_path);

    CHECK(json_diff.empty());
    if (!json_diff.empty()) {
        fs::path diff_file_path =
          TEST_DIR() / "result" / (name + "_pattern_table_diff.json");
        mizcore::write_json_file(json_diff, diff_file_path);
    } else {
        remove(result_file_path.c_str());
    }
}

} // namespace

TEST_CASE("execute arity parser")
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

    // erapsed time: 0.000165 [s]
    SUBCASE("ABCMIZ_0.miz") { TestPatternParser("abcmiz_0", symbol_table); }

    // erapsed time: 7.2e-05 [s]
    SUBCASE("ALTCAT_1.miz") { TestPatternParser("altcat_1", symbol_table); }

    // erapsed time: 0.00036 [s]
    SUBCASE("AOFA_A00.miz") { TestPatternParser("aofa_a00", symbol_table); }

    // erapsed time: 0.000861 [s]
    SUBCASE("JGRAPH_4.miz") { TestPatternParser("jgraph_4", symbol_table); }
}

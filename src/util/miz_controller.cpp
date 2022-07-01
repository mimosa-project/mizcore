#include <filesystem>
#include <fstream>
#include <iostream>

#include "ast_block.hpp"
#include "ast_token.hpp"
#include "config.hpp"
#include "error_table.hpp"
#include "miz_block_parser.hpp"
#include "miz_controller.hpp"
#include "miz_lexer_handler.hpp"
#include "spdlog/spdlog.h"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "token_table.hpp"
#include "vct_lexer_handler.hpp"

using mizcore::ErrorTable;
using mizcore::MizBlockParser;
using mizcore::MizController;
using mizcore::MizLexerHandler;
using mizcore::VctLexerHandler;
using mizcore::VctPath;

void
MizController::Exec(const char* mizpath)
{
    std::ifstream ifs_vct(VctPath());
    if (!ifs_vct) {
        spdlog::error("Failed to open vct file. The specified path: \"{}\"",
                      VctPath().string());
    }
    VctLexerHandler vct_handler(&ifs_vct);
    vct_handler.yylex();
    auto symbol_table = vct_handler.GetSymbolTable();
    std::ifstream ifs_miz(mizpath);
    if (!ifs_miz) {
        spdlog::error("Failed to open miz file. The specified path: \"{}\"",
                      mizpath);
    }
    MizLexerHandler miz_handler(&ifs_miz, symbol_table);
    miz_handler.yylex();
    token_table_ = miz_handler.GetTokenTable();
    error_table_ = std::make_shared<ErrorTable>();
    MizBlockParser miz_block_parser(token_table_, error_table_);
    miz_block_parser.Parse();
    ast_root_ = miz_block_parser.GetASTRoot();
}

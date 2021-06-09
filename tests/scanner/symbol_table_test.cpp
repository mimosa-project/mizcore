#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#undef yyFlexLexer
#define yyFlexLexer yyVctFlexLexer
#include <FlexLexer.h>
#undef yyFlexLexer

#include "doctest/doctest.h"
#include "symbol.hpp"
#include "symbol_table.hpp"
#include "vct_file_handler.hpp"

using std::ifstream;
using std::string;
namespace fs = std::filesystem;

using mizcore::Symbol;
using mizcore::SYMBOL_TYPE;
using mizcore::SymbolTable;
using mizcore::VctFileHandler;

const fs::path TEST_DATA_DIR = fs::path(__FILE__).parent_path() / "data";

TEST_CASE("symbol table test")
{
    fs::path mml_vct_path = TEST_DATA_DIR / "mml.vct";
    ifstream ifs(mml_vct_path.c_str());

    // Input file existence
    CHECK(ifs.good());

    VctFileHandler handler(&ifs);
    handler.yylex();

    std::shared_ptr<SymbolTable> table = handler.GetSymbolTable();

    // query map test
    SUBCASE("build query map for all symbols")
    {
        table->BuildQueryMap();
        Symbol* symbol = table->QueryLongestMatchSymbol("");
        CHECK(!symbol);

        symbol = table->QueryLongestMatchSymbol(".abc def ghi");
        CHECK(symbol);
        CHECK(symbol->GetText() == ".");
        CHECK(symbol->GetType() == SYMBOL_TYPE('O'));
        CHECK(symbol->GetPriority() == 100);

        symbol = table->QueryLongestMatchSymbol("..abc def ghi");
        CHECK(symbol);
        CHECK(symbol->GetText() == "..");
        CHECK(symbol->GetType() == SYMBOL_TYPE('O'));
        CHECK(symbol->GetPriority() == 100);

        symbol = table->QueryLongestMatchSymbol("||..abc def ghi");
        CHECK(symbol);
        CHECK(symbol->GetText() == "||..");
        CHECK(symbol->GetType() == SYMBOL_TYPE('K'));

        symbol = table->QueryLongestMatchSymbol("abss def ghi");
        CHECK(!symbol);

        symbol = table->QueryLongestMatchSymbol(",;:abc||def");
        CHECK(symbol);
        CHECK(symbol->GetText() == ",");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol(",||;:abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == ",");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("$1,abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == "$1");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("...||abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == "...");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("||abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == "||");
        CHECK(symbol->GetType() == SYMBOL_TYPE('O'));
        CHECK(symbol->GetPriority() == 100);

        symbol = table->QueryLongestMatchSymbol("= a");
        CHECK(symbol);
        CHECK(symbol->GetText() == "=");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("& sup I in I;");
        CHECK(symbol);
        CHECK(symbol->GetText() == "&");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));
    }

    SUBCASE("build query map for a few vocabulary files")
    {
        table->AddValidFileName("FINSEQ_4");
        table->AddValidFileName("COMPLEX1");
        table->AddValidFileName("INTEGRA9");
        table->BuildQueryMap();

        Symbol* symbol = table->QueryLongestMatchSymbol("");
        CHECK(!symbol);

        symbol = table->QueryLongestMatchSymbol(".abc def ghi");
        CHECK(!symbol);

        symbol = table->QueryLongestMatchSymbol("..abc def ghi");
        CHECK(symbol);
        CHECK(symbol->GetText() == "..");
        CHECK(symbol->GetType() == SYMBOL_TYPE('O'));
        CHECK(symbol->GetPriority() == 100);

        symbol = table->QueryLongestMatchSymbol("||..abc def ghi");
        CHECK(symbol);
        CHECK(symbol->GetText() == "||..");
        CHECK(symbol->GetType() == SYMBOL_TYPE('K'));

        symbol = table->QueryLongestMatchSymbol("abss def ghi");
        CHECK(!symbol);

        symbol = table->QueryLongestMatchSymbol(",;:abc||def");
        CHECK(symbol);
        CHECK(symbol->GetText() == ",");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol(",||;:abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == ",");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("$1,abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == "$1");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("...||abcdef");
        CHECK(symbol);
        CHECK(symbol->GetText() == "...");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("||abcdef");
        CHECK(!symbol);

        symbol = table->QueryLongestMatchSymbol("= a");
        CHECK(symbol);
        CHECK(symbol->GetText() == "=");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));

        symbol = table->QueryLongestMatchSymbol("& sup I in I;");
        CHECK(symbol);
        CHECK(symbol->GetText() == "&");
        CHECK(symbol->GetType() == SYMBOL_TYPE('S'));
    }
}

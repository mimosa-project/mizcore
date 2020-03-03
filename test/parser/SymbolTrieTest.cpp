#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <parser/SymbolTrie.hpp>
#include <spdlog/spdlog.h>
#include <cstdio>

using namespace std;
using namespace Mimosa;

namespace {
    std::string dirname(const std::string& path) {
        return path.substr(0, path.find_last_of("\\/"));
    }

    string root_dir = dirname(dirname(dirname(string(__FILE__))));
    string MML_VCT_PATH = root_dir + "/src/parser/data/mml.vct";
    string MML_TRIE_PATH = root_dir + "/src/parser/data/mml.trie";
}

TEST_CASE("SymbolTrie : article registration", "[parser][SymbolTrie]") {
    SymbolTrie symbol_trie;

    SECTION( "an article can be set appropriately" ) {
        symbol_trie.RegisterArticle("AAA");
        REQUIRE( symbol_trie.IsRegisteredArticle("AAA") );
    }

    SECTION("article registration is case-insensitive") {
        symbol_trie.RegisterArticle("aBc");
        REQUIRE( symbol_trie.IsRegisteredArticle("abc") );
        REQUIRE( symbol_trie.IsRegisteredArticle("aBc") );
        REQUIRE( symbol_trie.IsRegisteredArticle("ABC") );
    }

    SECTION( "registered articles are reset appropriately" ) {
        symbol_trie.RegisterArticle("AAA");
        symbol_trie.RegisterArticle("aBc");
        symbol_trie.Initialize();
        REQUIRE( !symbol_trie.IsRegisteredArticle("AAA") );
        REQUIRE( !symbol_trie.IsRegisteredArticle("ABC") );
    }
}

TEST_CASE("SymbolTrie : symbol access", "[parser][SymbolTrie]") {
    SymbolTrie symbol_trie;
    
    array<uint8_t, 2> value1 = {(uint8_t)'a', (uint8_t)1};
    array<uint8_t, 2> value2 = {(uint8_t)'b', (uint8_t)2};
    array<uint8_t, 2> value3 = {(uint8_t)'c', (uint8_t)3};

    symbol_trie.InsertSymbol("fin", value1);
    symbol_trie.InsertSymbol("find", value2);
    symbol_trie.InsertSymbol("finder", value3);

    SECTION("InsertSymbol works file") {
        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 3);
    }

    SECTION("FindLongestMatch: found case") {
        auto found = symbol_trie.FindLongestMatch("find a longest match");
        REQUIRE(found.first == string("find"));
        REQUIRE(found.second == value2);
    }

    SECTION("FindLongestMatch: not found case") {
        auto found = symbol_trie.FindLongestMatch("not found");
        REQUIRE(found.first == string(""));
    }

    SECTION("FindLongestMatchWithKeySize: found case") {
        auto found = symbol_trie.FindLongestMatchWithKeySize("find a longest match", 3);
        REQUIRE(found.first == string("fin"));
        REQUIRE(found.second == value1);
    }

    SECTION("FindLongestMatchWithKeySize: not found case") {
        auto found = symbol_trie.FindLongestMatchWithKeySize("find a longest match", 2);
        REQUIRE(found.first == string(""));
    }

    SECTION("EraseSymbol is work well") {
        symbol_trie.EraseSymbol("find");
        auto found = symbol_trie.FindLongestMatch("find a longest match");
        REQUIRE(found.first == string("fin"));
        REQUIRE(found.second == value1);
    }
}

TEST_CASE("SymbolTrie : build symbol trie from mml.vct", "[parser][SymbolTrie]") {
    SymbolTrie symbol_trie;

    SECTION("all symbols are registered in trie") {
        symbol_trie.BuildSymbolTrie(MML_VCT_PATH.c_str());
        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 9687);
    }

    SECTION("symbols only in given articles are registered in trie") {
        symbol_trie.RegisterArticle("AMISTD_1");    // 17+1 symbols
        symbol_trie.RegisterArticle("GROUP_1");     // 9 symbols
        symbol_trie.RegisterArticle("GROUP_2");     // 6 symbols
        symbol_trie.BuildSymbolTrie(MML_VCT_PATH.c_str());

        // HIDDEN : 4 symbols
        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 4+18+9+6);

        auto jump_in = symbol_trie.FindLongestMatch("jump_in");
        REQUIRE(jump_in.first == string("jump_in"));
        REQUIRE(jump_in.second == array<uint8_t,2>{'R',64});

        auto jumps_in = symbol_trie.FindLongestMatch("jumps_in");
        REQUIRE(jumps_in.first == string("jumps_in"));
        REQUIRE(jumps_in.second == array<uint8_t,2>{'R',64});

        auto unital = symbol_trie.FindLongestMatch("unital");
        REQUIRE(unital.first == string("unital"));
        REQUIRE(unital.second == array<uint8_t,2>{'V',64});

        auto Group = symbol_trie.FindLongestMatch("Group");
        REQUIRE(Group.first == string("Group"));
        REQUIRE(Group.second == array<uint8_t,2>{'M',64});

        auto power = symbol_trie.FindLongestMatch("power");
        REQUIRE(power.first == string("power"));
        REQUIRE(power.second == array<uint8_t,2>{'O',124});

        auto o1 = symbol_trie.FindLongestMatch("(1).");
        REQUIRE(o1.first == string("(1)."));
        REQUIRE(o1.second == array<uint8_t,2>{'O',128});
    }
}


TEST_CASE("SymbolTrie : load and save trie", "[parser][SymbolTrie]") {
    SymbolTrie symbol_trie;
    // REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 9687);
    // symbol_trie.SaveTrieMap(MML_TRIE_PATH.c_str());
    SECTION("load serialized trie file") {
        symbol_trie.LoadTrieMap(MML_TRIE_PATH.c_str());

        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 9687);
        auto jump_in = symbol_trie.FindLongestMatch("jump_in");
        REQUIRE(jump_in.first == string("jump_in"));
        REQUIRE(jump_in.second == array<uint8_t,2>{'R',64});
    }
    SECTION("save serialized trie file") {
        string tmpfile = root_dir + "/test/parser/tmp.trie";
        
        symbol_trie.BuildSymbolTrie(MML_VCT_PATH.c_str());
        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 9687);
        auto jump_in = symbol_trie.FindLongestMatch("jump_in");
        REQUIRE(jump_in.first == string("jump_in"));
        REQUIRE(jump_in.second == array<uint8_t,2>{'R',64});

        symbol_trie.SaveTrieMap(tmpfile.c_str());

        symbol_trie.Initialize();
        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 0);

        symbol_trie.LoadTrieMap(tmpfile.c_str());
        REQUIRE(symbol_trie.GetRegisteredSymbolNum() == 9687);
        
        remove(tmpfile.c_str());
    }
}

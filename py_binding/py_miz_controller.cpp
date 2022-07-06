#include "ast_block.hpp"
#include "ast_token.hpp"
#include "ast_type.hpp"

#include "error_table.hpp"
#include "miz_controller.hpp"
#include "token_table.hpp"
#include "symbol.hpp"

#include <string_view>
#include <pybind11/pybind11.h>

using mizcore::ASTBlock;
using mizcore::CommentToken;
using mizcore::ErrorTable;
using mizcore::IdentifierToken;
using mizcore::KeywordToken;
using mizcore::MizController;
using mizcore::NumeralToken;
using mizcore::SymbolToken;
using mizcore::TokenTable;
using mizcore::UnknownToken;
using mizcore::TOKEN_TYPE;
using mizcore::COMMENT_TYPE;
using mizcore::KEYWORD_TYPE;
using mizcore::SYMBOL_TYPE;
using mizcore::IDENTIFIER_TYPE;
using mizcore::Symbol;

namespace py = pybind11;

PYBIND11_MODULE(py_miz_controller, m)
{
    py::class_<TOKEN_TYPE, std::shared_ptr<TOKEN_TYPE>>(m, "TOKEN_TYPE");
    py::class_<Symbol, std::shared_ptr<Symbol>>(m, "Symbol")
      .def(py::init<std::string_view, SYMBOL_TYPE, uint8_t>());

    // Token class
    py::class_<UnknownToken, std::shared_ptr<UnknownToken>>(m, "UnknownToken")
      .def(py::init<size_t, size_t, std::string_view>())
      .def("get_text", &UnknownToken::GetText)
      .def("get_token_type", &UnknownToken::GetTokenType);

    py::class_<NumeralToken, std::shared_ptr<NumeralToken>>(m, "NumeralToken")
      .def(py::init<size_t, size_t, std::string_view>())
      .def("get_text", &NumeralToken::GetText)
      .def("get_token_type", &NumeralToken::GetTokenType);

    py::class_<SymbolToken, std::shared_ptr<SymbolToken>>(m, "SymbolToken")
      .def(py::init<size_t, size_t, Symbol*>())
      .def("get_text", &SymbolToken::GetText)
      .def("get_token_type", &SymbolToken::GetTokenType);

    py::class_<IdentifierToken, std::shared_ptr<IdentifierToken>>(
      m, "IdentifierToken")
      .def(py::init<size_t, size_t, std::string_view, IDENTIFIER_TYPE>())
      .def("get_text", &IdentifierToken::GetText)
      .def("get_token_type", &IdentifierToken::GetTokenType)
      .def("get_identifier_type", &IdentifierToken::GetIdentifierType);

    py::class_<CommentToken, std::shared_ptr<CommentToken>>(m, "CommentToken")
      .def(py::init<size_t, size_t, std::string_view, COMMENT_TYPE>())
      .def("get_text", &CommentToken::GetText)
      .def("get_token_type", &CommentToken::GetTokenType)
      .def("get_comment_type", &CommentToken::GetCommentType);

    py::class_<KeywordToken, std::shared_ptr<KeywordToken>>(m, "KeywordToken")
      .def(py::init<size_t, size_t, KEYWORD_TYPE>())
      .def("get_text", &KeywordToken::GetText)
      .def("get_token_type", &KeywordToken::GetTokenType)
      .def("get_keyword_type", &KeywordToken::GetKeywordType);

    // TokenTable
    py::class_<TokenTable, std::shared_ptr<TokenTable>>(m, "TokenTable")
      .def(py::init<>())
      .def("get_token", &TokenTable::GetToken)
      .def("get_token_num", &TokenTable::GetTokenNum);

    // ASTBlock
    py::class_<ASTBlock, std::shared_ptr<ASTBlock>>(m, "ASTBlock");

    // ErrorTable
    py::class_<ErrorTable, std::shared_ptr<ErrorTable>>(m, "ErrorTable");

    // MizController
    py::class_<MizController, std::shared_ptr<MizController>>(m, "MizController")
      .def(py::init<>())
      .def("exec", &MizController::Exec)
      .def("get_token_table", &MizController::GetTokenTable)
      .def("get_ast_root", &MizController::GetASTRoot)
      .def("get_error_table", &MizController::GetErrorTable);
}
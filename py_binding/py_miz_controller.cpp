#include "ast_block.hpp"
#include "ast_token.hpp"
#include "ast_type.hpp"
#include "error_table.hpp"
#include "miz_controller.hpp"
#include "token_table.hpp"
#include "symbol.hpp"
#include "py_ast_element.hpp"
#include "py_ast_token.hpp"
#include "py_symbol_token.hpp"
#include "py_unknown_token.hpp"
#include "py_numeral_token.hpp"
#include "py_identifier_token.hpp"
#include "py_comment_token.hpp"
#include "py_keyword_token.hpp"

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
using mizcore::BLOCK_TYPE;
// using mizcore::COMMENT_TYPE;
// using mizcore::KEYWORD_TYPE;
// using mizcore::IDENTIFIER_TYPE;
// using mizcore::SYMBOL_TYPE;
// using mizcore::Symbol;
using mizcore::QueryTokenTypeText;
using mizcore::ASTElement;
using mizcore::ASTToken;
using mizcore::PyASTElement;
using mizcore::PyASTToken;
using mizcore::PySymbolToken;
using mizcore::PyUnknownToken;
using mizcore::PyNumeralToken;
using mizcore::PyIdentifierToken;
using mizcore::PyCommentToken;
using mizcore::PyKeywordToken;

namespace py = pybind11;

PYBIND11_MODULE(py_miz_controller, m)
{
    m.def("query_token_type_text", &QueryTokenTypeText);

    py::enum_<TOKEN_TYPE>(m, "TOKEN_TYPE", py::arithmetic())
        .value("UNKNOWN", TOKEN_TYPE::UNKNOWN)
        .value("NUMERAL", TOKEN_TYPE::NUMERAL)
        .value("SYMBOL", TOKEN_TYPE::SYMBOL)
        .value("IDENTIFIER", TOKEN_TYPE::IDENTIFIER)
        .value("KEYWORD", TOKEN_TYPE::KEYWORD)
        .value("COMMENT", TOKEN_TYPE::COMMENT)
        .export_values();


    py::class_<ASTElement, PyASTElement, std::shared_ptr<ASTElement>>(m, "ASTElement")
      .def(py::init<>())
      .def("get_element_type", &ASTElement::GetElementType);

    py::class_<ASTToken, ASTElement, PyASTToken, std::shared_ptr<ASTToken>>(m, "ASTToken")
      .def(py::init<size_t, size_t>())
      .def("get_text", &ASTToken::GetText)
      .def("get_token_type", &ASTToken::GetTokenType);

    // py::class_<Symbol, std::shared_ptr<Symbol>>(m, "Symbol")
    //   .def(py::init<std::string_view, SYMBOL_TYPE, uint8_t>())
    //   .def("get_text", &Symbol::GetText);

    // Token class
    py::class_<UnknownToken,ASTToken, PyUnknownToken, std::shared_ptr<UnknownToken>>(m, "UnknownToken")
      // .def(py::init<size_t, size_t, std::string_view>())
      .def("get_text", &UnknownToken::GetText)
      .def("get_token_type", &UnknownToken::GetTokenType);

    py::class_<NumeralToken,ASTToken, PyNumeralToken, std::shared_ptr<NumeralToken>>(m, "NumeralToken")
      // .def(py::init<size_t, size_t, std::string_view>())
      .def("get_text", &NumeralToken::GetText)
      .def("get_token_type", &NumeralToken::GetTokenType);

    py::class_<SymbolToken, ASTToken, PySymbolToken, std::shared_ptr<SymbolToken>>(m, "SymbolToken")
      // .def(py::init<size_t, size_t, Symbol*>())
      .def("get_text", &SymbolToken::GetText)
      .def("get_token_type", &SymbolToken::GetTokenType);

    py::class_<IdentifierToken, ASTToken, PyIdentifierToken, std::shared_ptr<IdentifierToken>>(
      m, "IdentifierToken")
      // .def(py::init<size_t, size_t, std::string_view, IDENTIFIER_TYPE>())
      .def("get_text", &IdentifierToken::GetText)
      .def("get_token_type", &IdentifierToken::GetTokenType)
      .def("get_identifier_type", &IdentifierToken::GetIdentifierType)
      .def("get_ref_token", &IdentifierToken::GetRefToken);

    py::class_<CommentToken, ASTToken, PyCommentToken, std::shared_ptr<CommentToken>>(m, "CommentToken")
      // .def(py::init<size_t, size_t, std::string_view, COMMENT_TYPE>())
      .def("get_text", &CommentToken::GetText)
      .def("get_token_type", &CommentToken::GetTokenType)
      .def("get_comment_type", &CommentToken::GetCommentType);

    py::class_<KeywordToken, ASTToken, PyKeywordToken, std::shared_ptr<KeywordToken>>(m, "KeywordToken")
      // .def(py::init<size_t, size_t, KEYWORD_TYPE>())
      .def("get_text", &KeywordToken::GetText)
      .def("get_token_type", &KeywordToken::GetTokenType)
      .def("get_keyword_type", &KeywordToken::GetKeywordType);

    // TokenTable
    py::class_<TokenTable, std::shared_ptr<TokenTable>>(m, "TokenTable")
      .def(py::init<>())
      .def("get_token", &TokenTable::GetToken)
      .def("get_token_num", &TokenTable::GetTokenNum);

    // ASTBlock
    py::class_<ASTBlock, std::shared_ptr<ASTBlock>>(m, "ASTBlock")
      .def(py::init<BLOCK_TYPE>())
      .def("get_first_token", &ASTBlock::GetFirstToken)
      .def("get_child_component_num", &ASTBlock::GetChildComponentNum)
      .def("get_child_component", &ASTBlock::GetChildComponent);

    // ErrorTable
    py::class_<ErrorTable, std::shared_ptr<ErrorTable>>(m, "ErrorTable")
      .def(py::init<>())
      .def("log_errors", &ErrorTable::LogErrors);

    // MizController
    py::class_<MizController, std::shared_ptr<MizController>>(m, "MizController")
      .def(py::init<>())
      .def("exec", &MizController::Exec)
      .def("get_token_table", &MizController::GetTokenTable)
      .def("get_ast_root", &MizController::GetASTRoot)
      .def("get_error_table", &MizController::GetErrorTable);
}
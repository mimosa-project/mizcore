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
using mizcore::IDENTIFIER_TYPE;
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

    py::enum_<IDENTIFIER_TYPE>(m, "IDENTIFIER_TYPE", py::arithmetic())
        .value("UNKNOWN", IDENTIFIER_TYPE::UNKNOWN)
        .value("LABEL", IDENTIFIER_TYPE::LABEL)
        .value("VARIABLE", IDENTIFIER_TYPE::VARIABLE)
        .value("FILENAME", IDENTIFIER_TYPE::FILENAME)
        .value("SCHEME", IDENTIFIER_TYPE::SCHEME)
        .value("PREDICATE", IDENTIFIER_TYPE::PREDICATE)
        .value("FUNCTOR", IDENTIFIER_TYPE::FUNCTOR)
        .value("RESERVED", IDENTIFIER_TYPE::RESERVED)
        .export_values();

    py::class_<ASTElement, PyASTElement, std::shared_ptr<ASTElement>>(m, "ASTElement")
      .def_property_readonly("element_type", &ASTElement::GetElementType);

    py::class_<ASTToken, ASTElement, PyASTToken, std::shared_ptr<ASTToken>>(m, "ASTToken")
      .def_property_readonly("text", &ASTToken::GetText)
      .def_property_readonly("token_type", &ASTToken::GetTokenType)
      .def_property_readonly("ref_token", &ASTToken::GetRefToken, py::return_value_policy::reference);

    // Token class
    py::class_<UnknownToken,ASTToken, PyUnknownToken, std::shared_ptr<UnknownToken>>(m, "UnknownToken");

    py::class_<NumeralToken,ASTToken, PyNumeralToken, std::shared_ptr<NumeralToken>>(m, "NumeralToken");

    py::class_<SymbolToken, ASTToken, PySymbolToken, std::shared_ptr<SymbolToken>>(m, "SymbolToken");

    py::class_<IdentifierToken, ASTToken, PyIdentifierToken, std::shared_ptr<IdentifierToken>>(
      m, "IdentifierToken")
      .def_property_readonly("identifier_type", &IdentifierToken::GetIdentifierType);

    py::class_<CommentToken, ASTToken, PyCommentToken, std::shared_ptr<CommentToken>>(m, "CommentToken");

    py::class_<KeywordToken, ASTToken, PyKeywordToken, std::shared_ptr<KeywordToken>>(m, "KeywordToken");

    // TokenTable
    py::class_<TokenTable, std::shared_ptr<TokenTable>>(m, "TokenTable")
      .def("token", &TokenTable::GetToken, py::return_value_policy::reference)
      .def_property_readonly("token_num", &TokenTable::GetTokenNum);

    // ErrorTable
    py::class_<ErrorTable, std::shared_ptr<ErrorTable>>(m, "ErrorTable")
      .def("log_errors", &ErrorTable::LogErrors);

    // MizController
    py::class_<MizController, std::shared_ptr<MizController>>(m, "MizController")
      .def(py::init<>())
      .def("exec", &MizController::Exec)
      .def_property_readonly("token_table", &MizController::GetTokenTable)
      .def_property_readonly("ast_root", &MizController::GetASTRoot)
      .def_property_readonly("error_table", &MizController::GetErrorTable);
}
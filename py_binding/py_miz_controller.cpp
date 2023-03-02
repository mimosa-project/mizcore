#include "ast_block.hpp"
#include "ast_token.hpp"
#include "ast_type.hpp"
#include "ast_statement.hpp"
#include "error_table.hpp"
#include "miz_controller.hpp"
#include "token_table.hpp"
#include "py_ast_element.hpp"
#include "py_ast_token.hpp"
#include "py_ast_component.hpp"
#include "py_ast_block.hpp"
#include "py_ast_statement.hpp"
#include "py_unknown_token.hpp"
#include "py_numeral_token.hpp"
#include "py_symbol_token.hpp"
#include "py_identifier_token.hpp"
#include "py_keyword_token.hpp"
#include "py_comment_token.hpp"

#include <string_view>
#include <pybind11/pybind11.h>

using mizcore::TOKEN_TYPE;
using mizcore::ELEMENT_TYPE;
using mizcore::STATEMENT_TYPE;
using mizcore::BLOCK_TYPE;
using mizcore::SYMBOL_TYPE;
using mizcore::SPECIAL_SYMBOL_TYPE;
using mizcore::IDENTIFIER_TYPE;
using mizcore::COMMENT_TYPE;
using mizcore::KEYWORD_TYPE;

using mizcore::MizController;
using mizcore::ErrorTable;
using mizcore::TokenTable;

using mizcore::ASTElement;
using mizcore::ASTComponent;
using mizcore::ASTBlock;
using mizcore::ASTStatement;

using mizcore::ASTToken;
using mizcore::UnknownToken;
using mizcore::NumeralToken;
using mizcore::SymbolToken;
using mizcore::IdentifierToken;
using mizcore::KeywordToken;
using mizcore::CommentToken;

using mizcore::PyASTElement;
using mizcore::PyASTComponent;
using mizcore::PyASTBlock;
using mizcore::PyASTStatement;

using mizcore::PyASTToken;
using mizcore::PyUnknownToken;
using mizcore::PyNumeralToken;
using mizcore::PySymbolToken;
using mizcore::PyIdentifierToken;
using mizcore::PyKeywordToken;
using mizcore::PyCommentToken;

namespace py = pybind11;

PYBIND11_MODULE(py_miz_controller, m)
{
  py::enum_<TOKEN_TYPE>(m, "TokenType", py::arithmetic())
    .value("UNKNOWN", TOKEN_TYPE::UNKNOWN)
    .value("NUMERAL", TOKEN_TYPE::NUMERAL)
    .value("SYMBOL", TOKEN_TYPE::SYMBOL)
    .value("IDENTIFIER", TOKEN_TYPE::IDENTIFIER)
    .value("KEYWORD", TOKEN_TYPE::KEYWORD)
    .value("COMMENT", TOKEN_TYPE::COMMENT)
    .export_values();

  py::enum_<ELEMENT_TYPE>(m, "ElementType", py::arithmetic())
    .value("UNKNOWN", ELEMENT_TYPE::UNKNOWN)
    .value("BLOCK", ELEMENT_TYPE::BLOCK)
    .value("STATEMENT", ELEMENT_TYPE::STATEMENT)
    .value("TOKEN", ELEMENT_TYPE::TOKEN)
    .export_values();

  py::enum_<STATEMENT_TYPE>(m, "StatementType", py::arithmetic())
    .value("UNKNOWN", STATEMENT_TYPE::UNKNOWN)
    .value("ANTONYM", STATEMENT_TYPE::ANTONYM)
    .value("ASSOCIATIVITY", STATEMENT_TYPE::ASSOCIATIVITY)
    .value("ASSUME", STATEMENT_TYPE::ASSUME)
    .value("ASYMMETRY", STATEMENT_TYPE::ASYMMETRY)
    .value("ATTR", STATEMENT_TYPE::ATTR)
    .value("BEGIN_", STATEMENT_TYPE::BEGIN_)
    .value("CLUSTER", STATEMENT_TYPE::CLUSTER)
    .value("COHERENCE", STATEMENT_TYPE::COHERENCE)
    .value("COMMUTATIVITY", STATEMENT_TYPE::COMMUTATIVITY)
    .value("CONNECTEDNESS", STATEMENT_TYPE::CONNECTEDNESS)
    .value("CONSIDER", STATEMENT_TYPE::CONSIDER)
    .value("CONSISTENCY", STATEMENT_TYPE::CONSISTENCY)
    .value("CONSTRUCTORS", STATEMENT_TYPE::CONSTRUCTORS)
    .value("CORRECTNESS", STATEMENT_TYPE::CORRECTNESS)
    .value("DEFFUNC", STATEMENT_TYPE::DEFFUNC)
    .value("DEFINITIONS", STATEMENT_TYPE::DEFINITIONS)
    .value("DEFPRED", STATEMENT_TYPE::DEFPRED)
    .value("EMPTY", STATEMENT_TYPE::EMPTY)
    .value("ENVIRON", STATEMENT_TYPE::ENVIRON)
    .value("EX", STATEMENT_TYPE::EX)
    .value("EXISTENCE", STATEMENT_TYPE::EXISTENCE)
    .value("EXPANSIONS", STATEMENT_TYPE::EXPANSIONS)
    .value("EQUALITIES", STATEMENT_TYPE::EQUALITIES)
    .value("FOR", STATEMENT_TYPE::FOR)
    .value("FUNC", STATEMENT_TYPE::FUNC)
    .value("GIVEN", STATEMENT_TYPE::GIVEN)
    .value("HENCE", STATEMENT_TYPE::HENCE)
    .value("IDEMPOTENCE", STATEMENT_TYPE::IDEMPOTENCE)
    .value("IDENTIFY", STATEMENT_TYPE::IDENTIFY)
    .value("INVOLUTIVENESS", STATEMENT_TYPE::INVOLUTIVENESS)
    .value("IRREFLEXIVITY", STATEMENT_TYPE::IRREFLEXIVITY)
    .value("LET", STATEMENT_TYPE::LET)
    .value("MODE", STATEMENT_TYPE::MODE)
    .value("NOTATIONS", STATEMENT_TYPE::NOTATIONS)
    .value("PER_CASES", STATEMENT_TYPE::PER_CASES)
    .value("PRED", STATEMENT_TYPE::PRED)
    .value("PROJECTIVITY", STATEMENT_TYPE::PROJECTIVITY)
    .value("RECONSIDER", STATEMENT_TYPE::RECONSIDER)
    .value("REDUCE", STATEMENT_TYPE::REDUCE)
    .value("REDUCIBILITY", STATEMENT_TYPE::REDUCIBILITY)
    .value("REDEFINE", STATEMENT_TYPE::REDEFINE)
    .value("REFLEXIVITY", STATEMENT_TYPE::REFLEXIVITY)
    .value("REGISTRATIONS", STATEMENT_TYPE::REGISTRATIONS)
    .value("REQUIREMENTS", STATEMENT_TYPE::REQUIREMENTS)
    .value("RESERVE", STATEMENT_TYPE::RESERVE)
    .value("SCHEME", STATEMENT_TYPE::SCHEME)
    .value("SCHEMES", STATEMENT_TYPE::SCHEMES)
    .value("SECTION", STATEMENT_TYPE::SECTION)
    .value("SET", STATEMENT_TYPE::SET)
    .value("SETHOOD", STATEMENT_TYPE::SETHOOD)
    .value("STRUCT", STATEMENT_TYPE::STRUCT)
    .value("SYMMETRY", STATEMENT_TYPE::SYMMETRY)
    .value("SYNONYM", STATEMENT_TYPE::SYNONYM)
    .value("TAKE", STATEMENT_TYPE::TAKE)
    .value("THEOREM", STATEMENT_TYPE::THEOREM)
    .value("THEOREMS", STATEMENT_TYPE::THEOREMS)
    .value("THUS", STATEMENT_TYPE::THUS)
    .value("TRANSITIVITY", STATEMENT_TYPE::TRANSITIVITY)
    .value("UNIQUENESS", STATEMENT_TYPE::UNIQUENESS)
    .value("VOCABULARIES", STATEMENT_TYPE::VOCABULARIES)
    .export_values();

  py::enum_<BLOCK_TYPE>(m, "BlockType", py::arithmetic())
    .value("UNKNOWN", BLOCK_TYPE::UNKNOWN)
    .value("ROOT", BLOCK_TYPE::ROOT)
    .value("DEFINITION", BLOCK_TYPE::DEFINITION)
    .value("REGISTRATION", BLOCK_TYPE::REGISTRATION)
    .value("NOTATION", BLOCK_TYPE::NOTATION)
    .value("SCHEME", BLOCK_TYPE::SCHEME)
    .value("CASE", BLOCK_TYPE::CASE)
    .value("SUPPOSE", BLOCK_TYPE::SUPPOSE)
    .value("HEREBY", BLOCK_TYPE::HEREBY)
    .value("NOW", BLOCK_TYPE::NOW)
    .value("PROOF", BLOCK_TYPE::PROOF)
    .export_values();

  py::enum_<SYMBOL_TYPE>(m, "SymbolType", py::arithmetic())
    .value("UNKNOWN", SYMBOL_TYPE::UNKNOWN)
    .value("PREDICATE", SYMBOL_TYPE::PREDICATE)
    .value("FUNCTOR", SYMBOL_TYPE::FUNCTOR)
    .value("MODE", SYMBOL_TYPE::MODE)
    .value("STRUCTURE", SYMBOL_TYPE::STRUCTURE)
    .value("SELECTOR", SYMBOL_TYPE::SELECTOR)
    .value("ATTRIBUTE", SYMBOL_TYPE::ATTRIBUTE)
    .value("LEFT_FUNCTOR_BRACKET", SYMBOL_TYPE::LEFT_FUNCTOR_BRACKET)
    .value("RIGHT_FUNCTOR_BRACKET", SYMBOL_TYPE::RIGHT_FUNCTOR_BRACKET)
    .value("SPECIAL", SYMBOL_TYPE::SPECIAL)
    .export_values();

  py::enum_<SPECIAL_SYMBOL_TYPE>(m, "SpecialSymbolType", py::arithmetic())
    .value("UNKNOWN", SPECIAL_SYMBOL_TYPE::UNKNOWN)
    .value("COMMA", SPECIAL_SYMBOL_TYPE::COMMA)
    .value("SEMICOLON", SPECIAL_SYMBOL_TYPE::SEMICOLON)
    .value("COLON", SPECIAL_SYMBOL_TYPE::COLON)
    .value("LEFT_PARENTHESIS", SPECIAL_SYMBOL_TYPE::LEFT_PARENTHESIS)
    .value("RIGHT_PARENTHESIS", SPECIAL_SYMBOL_TYPE::RIGHT_PARENTHESIS)
    .value("LEFT_BRACKET", SPECIAL_SYMBOL_TYPE::LEFT_BRACKET)
    .value("RIGHT_BRACKET", SPECIAL_SYMBOL_TYPE::RIGHT_BRACKET)
    .value("LEFT_BRACE", SPECIAL_SYMBOL_TYPE::LEFT_BRACE)
    .value("RIGHT_BRACE", SPECIAL_SYMBOL_TYPE::RIGHT_BRACE)
    .value("EQUAL", SPECIAL_SYMBOL_TYPE::EQUAL)
    .value("AND", SPECIAL_SYMBOL_TYPE::AND)
    .value("ARROW", SPECIAL_SYMBOL_TYPE::ARROW)
    .value("DOT_EQUAL", SPECIAL_SYMBOL_TYPE::DOT_EQUAL)
    .value("THREE_DOTS", SPECIAL_SYMBOL_TYPE::THREE_DOTS)
    .value("DOLLAR_1", SPECIAL_SYMBOL_TYPE::DOLLAR_1)
    .value("DOLLAR_2", SPECIAL_SYMBOL_TYPE::DOLLAR_2)
    .value("DOLLAR_3", SPECIAL_SYMBOL_TYPE::DOLLAR_3)
    .value("DOLLAR_4", SPECIAL_SYMBOL_TYPE::DOLLAR_4)
    .value("DOLLAR_5", SPECIAL_SYMBOL_TYPE::DOLLAR_5)
    .value("DOLLAR_6", SPECIAL_SYMBOL_TYPE::DOLLAR_6)
    .value("DOLLAR_7", SPECIAL_SYMBOL_TYPE::DOLLAR_7)
    .value("DOLLAR_8", SPECIAL_SYMBOL_TYPE::DOLLAR_8)
    .value("DOLLAR_9", SPECIAL_SYMBOL_TYPE::DOLLAR_9)
    .value("DOLLAR_10", SPECIAL_SYMBOL_TYPE::DOLLAR_10)
    .value("LEFT_FIELD", SPECIAL_SYMBOL_TYPE::LEFT_FIELD)
    .value("RIGHT_FIELD", SPECIAL_SYMBOL_TYPE::RIGHT_FIELD)
    .export_values();

  py::enum_<IDENTIFIER_TYPE>(m, "IdentifierType", py::arithmetic())
    .value("UNKNOWN", IDENTIFIER_TYPE::UNKNOWN)
    .value("LABEL", IDENTIFIER_TYPE::LABEL)
    .value("VARIABLE", IDENTIFIER_TYPE::VARIABLE)
    .value("FILENAME", IDENTIFIER_TYPE::FILENAME)
    .value("SCHEME", IDENTIFIER_TYPE::SCHEME)
    .value("PREDICATE", IDENTIFIER_TYPE::PREDICATE)
    .value("FUNCTOR", IDENTIFIER_TYPE::FUNCTOR)
    .value("RESERVED", IDENTIFIER_TYPE::RESERVED)
    .export_values();

  py::enum_<COMMENT_TYPE>(m, "CommentType", py::arithmetic())
    .value("UNKNOWN", COMMENT_TYPE::UNKNOWN)
    .value("DOUBLE", COMMENT_TYPE::DOUBLE)
    .value("TRIPLE", COMMENT_TYPE::TRIPLE)
    .export_values();

  py::enum_<KEYWORD_TYPE>(m, "KeywordType", py::arithmetic())
    .value("UNKNOWN", KEYWORD_TYPE::UNKNOWN)
    .value("ACCORDING", KEYWORD_TYPE::ACCORDING)
    .value("AGGREGATE", KEYWORD_TYPE::AGGREGATE)
    .value("ALL", KEYWORD_TYPE::ALL)
    .value("AND", KEYWORD_TYPE::AND)
    .value("ANTONYM", KEYWORD_TYPE::ANTONYM)
    .value("ARE", KEYWORD_TYPE::ARE)
    .value("AS", KEYWORD_TYPE::AS)
    .value("ASSOCIATIVITY", KEYWORD_TYPE::ASSOCIATIVITY)
    .value("ASSUME", KEYWORD_TYPE::ASSUME)
    .value("ASYMMETRY", KEYWORD_TYPE::ASYMMETRY)
    .value("ATTR", KEYWORD_TYPE::ATTR)
    .value("BE", KEYWORD_TYPE::BE)
    .value("BEGIN_", KEYWORD_TYPE::BEGIN_)
    .value("BEING", KEYWORD_TYPE::BEING)
    .value("BY", KEYWORD_TYPE::BY)
    .value("CANCELED", KEYWORD_TYPE::CANCELED)
    .value("CASE", KEYWORD_TYPE::CASE)
    .value("CASES", KEYWORD_TYPE::CASES)
    .value("CLUSTER", KEYWORD_TYPE::CLUSTER)
    .value("COHERENCE", KEYWORD_TYPE::COHERENCE)
    .value("COMMUTATIVITY", KEYWORD_TYPE::COMMUTATIVITY)
    .value("COMPATIBILITY", KEYWORD_TYPE::COMPATIBILITY)
    .value("CONNECTEDNESS", KEYWORD_TYPE::CONNECTEDNESS)
    .value("CONSIDER", KEYWORD_TYPE::CONSIDER)
    .value("CONSISTENCY", KEYWORD_TYPE::CONSISTENCY)
    .value("CONSTRUCTORS", KEYWORD_TYPE::CONSTRUCTORS)
    .value("CONTRADICTION", KEYWORD_TYPE::CONTRADICTION)
    .value("CORRECTNESS", KEYWORD_TYPE::CORRECTNESS)
    .value("DEF", KEYWORD_TYPE::DEF)
    .value("DEFFUNC", KEYWORD_TYPE::DEFFUNC)
    .value("DEFINE", KEYWORD_TYPE::DEFINE)
    .value("DEFINITION", KEYWORD_TYPE::DEFINITION)
    .value("DEFINITIONS", KEYWORD_TYPE::DEFINITIONS)
    .value("DEFPRED", KEYWORD_TYPE::DEFPRED)
    .value("DO", KEYWORD_TYPE::DO)
    .value("DOES", KEYWORD_TYPE::DOES)
    .value("END", KEYWORD_TYPE::END)
    .value("ENVIRON", KEYWORD_TYPE::ENVIRON)
    .value("EQUALITIES", KEYWORD_TYPE::EQUALITIES)
    .value("EQUALS", KEYWORD_TYPE::EQUALS)
    .value("EX", KEYWORD_TYPE::EX)
    .value("EXACTLY", KEYWORD_TYPE::EXACTLY)
    .value("EXISTENCE", KEYWORD_TYPE::EXISTENCE)
    .value("EXPANSIONS", KEYWORD_TYPE::EXPANSIONS)
    .value("FOR", KEYWORD_TYPE::FOR)
    .value("FROM", KEYWORD_TYPE::FROM)
    .value("FUNC", KEYWORD_TYPE::FUNC)
    .value("GIVEN", KEYWORD_TYPE::GIVEN)
    .value("HENCE", KEYWORD_TYPE::HENCE)
    .value("HEREBY", KEYWORD_TYPE::HEREBY)
    .value("HOLDS", KEYWORD_TYPE::HOLDS)
    .value("IDEMPOTENCE", KEYWORD_TYPE::IDEMPOTENCE)
    .value("IDENTIFY", KEYWORD_TYPE::IDENTIFY)
    .value("IF", KEYWORD_TYPE::IF)
    .value("IFF", KEYWORD_TYPE::IFF)
    .value("IMPLIES", KEYWORD_TYPE::IMPLIES)
    .value("INVOLUTIVENESS", KEYWORD_TYPE::INVOLUTIVENESS)
    .value("IRREFLEXIVITY", KEYWORD_TYPE::IRREFLEXIVITY)
    .value("IS", KEYWORD_TYPE::IS)
    .value("IT", KEYWORD_TYPE::IT)
    .value("LET", KEYWORD_TYPE::LET)
    .value("MEANS", KEYWORD_TYPE::MEANS)
    .value("MODE", KEYWORD_TYPE::MODE)
    .value("NON", KEYWORD_TYPE::NON)
    .value("NOT", KEYWORD_TYPE::NOT)
    .value("NOTATION", KEYWORD_TYPE::NOTATION)
    .value("NOTATIONS", KEYWORD_TYPE::NOTATIONS)
    .value("NOW", KEYWORD_TYPE::NOW)
    .value("OF", KEYWORD_TYPE::OF)
    .value("OR", KEYWORD_TYPE::OR)
    .value("OTHERWISE", KEYWORD_TYPE::OTHERWISE)
    .value("OVER", KEYWORD_TYPE::OVER)
    .value("PER", KEYWORD_TYPE::PER)
    .value("PRED", KEYWORD_TYPE::PRED)
    .value("PREFIX", KEYWORD_TYPE::PREFIX)
    .value("PROJECTIVITY", KEYWORD_TYPE::PROJECTIVITY)
    .value("PROOF", KEYWORD_TYPE::PROOF)
    .value("PROVIDED", KEYWORD_TYPE::PROVIDED)
    .value("QUA", KEYWORD_TYPE::QUA)
    .value("RECONSIDER", KEYWORD_TYPE::RECONSIDER)
    .value("REDUCE", KEYWORD_TYPE::REDUCE)
    .value("REDUCIBILITY", KEYWORD_TYPE::REDUCIBILITY)
    .value("REDEFINE", KEYWORD_TYPE::REDEFINE)
    .value("REFLEXIVITY", KEYWORD_TYPE::REFLEXIVITY)
    .value("REGISTRATION", KEYWORD_TYPE::REGISTRATION)
    .value("REGISTRATIONS", KEYWORD_TYPE::REGISTRATIONS)
    .value("REQUIREMENTS", KEYWORD_TYPE::REQUIREMENTS)
    .value("RESERVE", KEYWORD_TYPE::RESERVE)
    .value("SCH", KEYWORD_TYPE::SCH)
    .value("SCHEME", KEYWORD_TYPE::SCHEME)
    .value("SCHEMES", KEYWORD_TYPE::SCHEMES)
    .value("SECTION", KEYWORD_TYPE::SECTION)
    .value("SELECTOR", KEYWORD_TYPE::SELECTOR)
    .value("SET", KEYWORD_TYPE::SET)
    .value("SETHOOD", KEYWORD_TYPE::SETHOOD)
    .value("ST", KEYWORD_TYPE::ST)
    .value("STRUCT", KEYWORD_TYPE::STRUCT)
    .value("SUCH", KEYWORD_TYPE::SUCH)
    .value("SUPPOSE", KEYWORD_TYPE::SUPPOSE)
    .value("SYMMETRY", KEYWORD_TYPE::SYMMETRY)
    .value("SYNONYM", KEYWORD_TYPE::SYNONYM)
    .value("TAKE", KEYWORD_TYPE::TAKE)
    .value("THAT", KEYWORD_TYPE::THAT)
    .value("THE", KEYWORD_TYPE::THE)
    .value("THEN", KEYWORD_TYPE::THEN)
    .value("THEOREM", KEYWORD_TYPE::THEOREM)
    .value("THEOREMS", KEYWORD_TYPE::THEOREMS)
    .value("THESIS", KEYWORD_TYPE::THESIS)
    .value("THUS", KEYWORD_TYPE::THUS)
    .value("TO", KEYWORD_TYPE::TO)
    .value("TRANSITIVITY", KEYWORD_TYPE::TRANSITIVITY)
    .value("UNIQUENESS", KEYWORD_TYPE::UNIQUENESS)
    .value("VOCABULARIES", KEYWORD_TYPE::VOCABULARIES)
    .value("WHEN", KEYWORD_TYPE::WHEN)
    .value("WHERE", KEYWORD_TYPE::WHERE)
    .value("WITH", KEYWORD_TYPE::WITH)
    .value("WRT", KEYWORD_TYPE::WRT)
    .export_values();

  py::class_<ASTElement, PyASTElement, std::shared_ptr<ASTElement>>(m, "ASTElement")
    .def_property_readonly("element_type", &ASTElement::GetElementType);

  py::class_<ASTComponent, ASTElement, PyASTComponent, std::shared_ptr<ASTComponent>>(m, "ASTComponent")
    .def_property_readonly("parent", &ASTComponent::GetParent, py::return_value_policy::reference)
    .def("is_error", &ASTComponent::IsError)
    .def_property_readonly("range_first_token", &ASTComponent::GetRangeFirstToken, py::return_value_policy::reference)
    .def_property_readonly("range_last_token", &ASTComponent::GetRangeLastToken, py::return_value_policy::reference);

  py::class_<ASTStatement, ASTComponent, PyASTStatement, std::shared_ptr<ASTStatement>>(m, "ASTStatement")
    .def_property_readonly("statement_type", &ASTStatement::GetStatementType);

  py::class_<ASTBlock, ASTComponent, PyASTBlock, std::shared_ptr<ASTBlock>>(m, "ASTBlock")
    .def_property_readonly("block_type", &ASTBlock::GetBlockType)
    .def_property_readonly("first_token", &ASTBlock::GetFirstToken, py::return_value_policy::reference)
    .def_property_readonly("last_token", &ASTBlock::GetLastToken, py::return_value_policy::reference)
    .def_property_readonly("semicolon_token", &ASTBlock::GetSemicolonToken, py::return_value_policy::reference)
    .def_property_readonly("child_component_num", &ASTBlock::GetChildComponentNum)
    .def("child_component", &ASTBlock::GetChildComponent, py::return_value_policy::reference)
    .def("child_block", &ASTBlock::GetChildBlock, py::return_value_policy::reference)
    .def("child_statement", &ASTBlock::GetChildStatement, py::return_value_policy::reference);

  py::class_<ASTToken, ASTElement, PyASTToken, std::shared_ptr<ASTToken>>(m, "ASTToken")
    .def_property_readonly("id", &ASTToken::GetId)
    .def_property_readonly("line_number", &ASTToken::GetLineNumber)
    .def_property_readonly("column_number", &ASTToken::GetColumnNumber)
    .def_property_readonly("text", &ASTToken::GetText)
    .def_property_readonly("token_type", &ASTToken::GetTokenType)
    .def_property_readonly("ref_token", &ASTToken::GetRefToken, py::return_value_policy::reference);

  py::class_<UnknownToken,ASTToken, PyUnknownToken, std::shared_ptr<UnknownToken>>(m, "UnknownToken");

  py::class_<NumeralToken,ASTToken, PyNumeralToken, std::shared_ptr<NumeralToken>>(m, "NumeralToken");

  py::class_<SymbolToken, ASTToken, PySymbolToken, std::shared_ptr<SymbolToken>>(m, "SymbolToken")
    .def_property_readonly("symbol_type", &SymbolToken::GetSymbolType)
    .def_property_readonly("special_symbol_type", &SymbolToken::GetSpecialSymbolType);

  py::class_<IdentifierToken, ASTToken, PyIdentifierToken, std::shared_ptr<IdentifierToken>>(
    m, "IdentifierToken")
    .def_property_readonly("identifier_type", &IdentifierToken::GetIdentifierType);

  py::class_<CommentToken, ASTToken, PyCommentToken, std::shared_ptr<CommentToken>>(m, "CommentToken")
    .def_property_readonly("comment_type", &CommentToken::GetCommentType);

  py::class_<KeywordToken, ASTToken, PyKeywordToken, std::shared_ptr<KeywordToken>>(m, "KeywordToken")
    .def_property_readonly("keyword_type", &KeywordToken::GetKeywordType);

  py::class_<TokenTable, std::shared_ptr<TokenTable>>(m, "TokenTable")
    .def("token", &TokenTable::GetToken, py::return_value_policy::reference)
    .def_property_readonly("token_num", &TokenTable::GetTokenNum)
    .def_property_readonly("last_token", &TokenTable::GetLastToken);

  py::class_<ErrorTable, std::shared_ptr<ErrorTable>>(m, "ErrorTable")
    .def("log_errors", &ErrorTable::LogErrors);

  py::class_<MizController, std::shared_ptr<MizController>>(m, "MizController")
    .def(py::init<>())
    .def("exec_file", &MizController::ExecFile)
    .def("exec_buffer", &MizController::ExecBuffer)
    .def("is_abs_mode", &MizController::IsABSMode)
    .def_property_readonly("token_table", &MizController::GetTokenTable)
    .def_property_readonly("ast_root", &MizController::GetASTRoot)
    .def_property_readonly("error_table", &MizController::GetErrorTable);
}
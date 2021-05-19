#include <cassert>
#include <cstring>
#include <ostream>
#include <string>
#include <map>
#include <iomanip>
#include <new>

#include "token.hpp"
#include "symbol.hpp"

using std::string;
using std::map;
using emcore::mizar::TOKEN_TYPE;
using emcore::mizar::Token;

Token::Token(size_t line_number, size_t column_number, TOKEN_TYPE type,
             const char* text, Symbol* symbol)
: line_number_(line_number),
  column_number_(column_number),
  type_(type),
  text_(nullptr)
{
    if (text) {
        text_ = strdup(text);
        if (!text_) {
            throw std::bad_alloc();
        }
    }
    if (symbol) {
        symbol_ = symbol;
    }
}

Token::Token(const Token& rhs)
: line_number_(rhs.line_number_),
  column_number_(rhs.column_number_),
  type_(rhs.type_),
  text_(nullptr)
{
    if (rhs.type_ == TOKEN_TYPE::SYMBOL) {
        symbol_ = rhs.symbol_;
    } else if (rhs.text_) {
        text_ = strdup(rhs.text_);
        if (!text_) {
            throw std::bad_alloc();
        }
    }
}

Token::Token(Token&& rhs)
: line_number_(rhs.line_number_),
  column_number_(rhs.column_number_),
  type_(rhs.type_),
  text_(nullptr)
{
    if (rhs.type_ == TOKEN_TYPE::SYMBOL) {
        symbol_ = rhs.symbol_;
    } else if (rhs.text_) {
        text_ = rhs.text_;
        rhs.text_ = nullptr;
    }
}

Token& Token::operator=(const Token& rhs)
{
    if (this != &rhs) {
        line_number_ = rhs.line_number_;
        column_number_ = rhs.column_number_;
        type_ = rhs.type_;
        text_ = nullptr;

        if (rhs.type_ == TOKEN_TYPE::SYMBOL) {
            symbol_ = rhs.symbol_;
        } else if (rhs.text_) {
            text_ = strdup(rhs.text_);
            if (!text_) {
                throw std::bad_alloc();
            }
        }
    }
    return *this;
}

Token& Token::operator=(Token&& rhs)
{
    if (this != &rhs) {
        line_number_ = rhs.line_number_;
        column_number_ = rhs.column_number_;
        type_ = rhs.type_;
        text_ = nullptr;

        if (rhs.type_ == TOKEN_TYPE::SYMBOL) {
            symbol_ = rhs.symbol_;
        } else if (rhs.text_) {
            text_ = rhs.text_;
            rhs.text_ = nullptr;
        }
    }
    return *this;
}

Token::~Token()
{
    if (type_ != TOKEN_TYPE::SYMBOL) {
        free(text_);
    }
}

const char* Token::GetText() const
{
    if (type_ == TOKEN_TYPE::SYMBOL) {
        return symbol_->GetName();
    } else if (int(type_) >= int(TOKEN_TYPE::ACCORDING)) {
        return QueryTypeText(type_);
    } else {
        return text_;
    }
}

size_t Token::GetLength() const
{
    static size_t length_array[] = {
        0,0,0,0,0,0,0,0,0,9,
        9,3,3,7,3,2,13,6,9,4,
        2,5,5,2,8,4,5,7,9,13,
        13,13,8,11,12,13,11,3,7,6,
        10,11,7,2,4,3,7,6,2,7,
        9,3,4,4,5,5,6,5,11,8,
        2,3,7,14,13,2,2,3,5,4,
        3,3,8,9,3,2,2,9,4,3,
        4,6,12,5,8,3,10,6,12,8,
        11,12,13,12,7,3,6,7,7,8,
        3,7,2,6,4,7,8,7,4,4,
        3,4,7,8,6,4,2,12,10,12,
        4,5,4,3,
    };

    if (type_ == TOKEN_TYPE::SYMBOL) {
        return symbol_->GetLength();
    } else if (int(type_) >= int(TOKEN_TYPE::ACCORDING)) {
        return length_array[(size_t)type_];
    } else {
        return strlen(text_);
    }
}

void Token::Dump(std::ostream& os) const {
    os << "pos = (" << std::setw(4) << line_number_ << ", "
                    << std::setw(4) << column_number_ << "), "
       << "length = " << std::setw(3) << GetLength() << ", "
       << "type = " << std::setw(14);
    if (int(type_) < int(TOKEN_TYPE::ACCORDING)) {
        os << QueryTypeText(type_);
    } else {
        os << "KEYWORD";
    }
    os << ", "
       << "text = \"" << GetText() << "\"";
    if (type_ == TOKEN_TYPE::SYMBOL) {
        os << ", symbol_type = " << (char)symbol_->GetType()
           << ", priority = " << (int)symbol_->GetPriority();
    }
}

const char* Token::QueryTypeText(TOKEN_TYPE type)
{
    static string type_text[] = {
        "UNKNOWN",
        "NUMERAL",
        "SYMBOL",
        "IDENTIFIER",
        "LABEL",
        "VARIABLE",
        "FILENAME",
        "DOUBLE_COMMENT",
        "TRIPLE_COMMENT",
        "according",
        "aggregate",
        "all",
        "and",
        "antonym",
        "are",
        "as",
        "associativity",
        "assume",
        "asymmetry",
        "attr",
        "be",
        "begin",
        "being",
        "by",
        "canceled",
        "case",
        "cases",
        "cluster",
        "coherence",
        "commutativity",
        "compatibility",
        "connectedness",
        "consider",
        "consistency",
        "constructors",
        "contradiction",
        "correctness",
        "def",
        "deffunc",
        "define",
        "definition",
        "definitions",
        "defpred",
        "do",
        "does",
        "end",
        "environ",
        "equals",
        "ex",
        "exactly",
        "existence",
        "for",
        "from",
        "func",
        "given",
        "hence",
        "hereby",
        "holds",
        "idempotence",
        "identify",
        "if",
        "iff",
        "implies",
        "involutiveness",
        "irreflexivity",
        "is",
        "it",
        "let",
        "means",
        "mode",
        "non",
        "not",
        "notation",
        "notations",
        "now",
        "of",
        "or",
        "otherwise",
        "over",
        "per",
        "pred",
        "prefix",
        "projectivity",
        "proof",
        "provided",
        "qua",
        "reconsider",
        "reduce",
        "reducibility",
        "redefine",
        "reflexivity",
        "registration",
        "registrations",
        "requirements",
        "reserve",
        "sch",
        "scheme",
        "schemes",
        "section",
        "selector",
        "set",
        "sethood",
        "st",
        "struct",
        "such",
        "suppose",
        "symmetry",
        "synonym",
        "take",
        "that",
        "the",
        "then",
        "theorem",
        "theorems",
        "thesis",
        "thus",
        "to",
        "transitivity",
        "uniqueness",
        "vocabularies",
        "when",
        "where",
        "with",
        "wrt",
    };

    return type_text[size_t(type)].c_str();
}

TOKEN_TYPE Token::QueryKeywordType(const char* text)
{
    static map<string, TOKEN_TYPE> keyword_map = 
    {
        {"according", TOKEN_TYPE::ACCORDING},
        {"aggregate", TOKEN_TYPE::AGGREGATE},
        {"all", TOKEN_TYPE::ALL},
        {"and", TOKEN_TYPE::AND},
        {"antonym", TOKEN_TYPE::ANTONYM},
        {"are", TOKEN_TYPE::ARE},
        {"as", TOKEN_TYPE::AS},
        {"associativity", TOKEN_TYPE::ASSOCIATIVITY},
        {"assume", TOKEN_TYPE::ASSUME},
        {"asymmetry", TOKEN_TYPE::ASYMMETRY},
        {"attr", TOKEN_TYPE::ATTR},
        {"be", TOKEN_TYPE::BE},
        {"begin", TOKEN_TYPE::BEGIN_},
        {"being", TOKEN_TYPE::BEING},
        {"by", TOKEN_TYPE::BY},
        {"canceled", TOKEN_TYPE::CANCELED},
        {"case", TOKEN_TYPE::CASE},
        {"cases", TOKEN_TYPE::CASES},
        {"cluster", TOKEN_TYPE::CLUSTER},
        {"coherence", TOKEN_TYPE::COHERENCE},
        {"commutativity", TOKEN_TYPE::COMMUTATIVITY},
        {"compatibility", TOKEN_TYPE::COMPATIBILITY},
        {"connectedness", TOKEN_TYPE::CONNECTEDNESS},
        {"consider", TOKEN_TYPE::CONSIDER},
        {"consistency", TOKEN_TYPE::CONSISTENCY},
        {"constructors", TOKEN_TYPE::CONSTRUCTORS},
        {"contradiction", TOKEN_TYPE::CONTRADICTION},
        {"correctness", TOKEN_TYPE::CORRECTNESS},
        {"def", TOKEN_TYPE::DEF},
        {"deffunc", TOKEN_TYPE::DEFFUNC},
        {"define", TOKEN_TYPE::DEFINE},
        {"definition", TOKEN_TYPE::DEFINITION},
        {"definitions", TOKEN_TYPE::DEFINITIONS},
        {"defpred", TOKEN_TYPE::DEFPRED},
        {"do", TOKEN_TYPE::DO},
        {"does", TOKEN_TYPE::DOES},
        {"end", TOKEN_TYPE::END},
        {"environ", TOKEN_TYPE::ENVIRON},
        {"equals", TOKEN_TYPE::EQUALS},
        {"ex", TOKEN_TYPE::EX},
        {"exactly", TOKEN_TYPE::EXACTLY},
        {"existence", TOKEN_TYPE::EXISTENCE},
        {"for", TOKEN_TYPE::FOR},
        {"from", TOKEN_TYPE::FROM},
        {"func", TOKEN_TYPE::FUNC},
        {"given", TOKEN_TYPE::GIVEN},
        {"hence", TOKEN_TYPE::HENCE},
        {"hereby", TOKEN_TYPE::HEREBY},
        {"holds", TOKEN_TYPE::HOLDS},
        {"idempotence", TOKEN_TYPE::IDEMPOTENCE},
        {"identify", TOKEN_TYPE::IDENTIFY},
        {"if", TOKEN_TYPE::IF},
        {"iff", TOKEN_TYPE::IFF},
        {"implies", TOKEN_TYPE::IMPLIES},
        {"involutiveness", TOKEN_TYPE::INVOLUTIVENESS},
        {"irreflexivity", TOKEN_TYPE::IRREFLEXIVITY},
        {"is", TOKEN_TYPE::IS},
        {"it", TOKEN_TYPE::IT},
        {"let", TOKEN_TYPE::LET},
        {"means", TOKEN_TYPE::MEANS},
        {"mode", TOKEN_TYPE::MODE},
        {"non", TOKEN_TYPE::NON},
        {"not", TOKEN_TYPE::NOT},
        {"notation", TOKEN_TYPE::NOTATION},
        {"notations", TOKEN_TYPE::NOTATIONS},
        {"now", TOKEN_TYPE::NOW},
        {"of", TOKEN_TYPE::OF},
        {"or", TOKEN_TYPE::OR},
        {"otherwise", TOKEN_TYPE::OTHERWISE},
        {"over", TOKEN_TYPE::OVER},
        {"per", TOKEN_TYPE::PER},
        {"pred", TOKEN_TYPE::PRED},
        {"prefix", TOKEN_TYPE::PREFIX},
        {"projectivity", TOKEN_TYPE::PROJECTIVITY},
        {"proof", TOKEN_TYPE::PROOF},
        {"provided", TOKEN_TYPE::PROVIDED},
        {"qua", TOKEN_TYPE::QUA},
        {"reconsider", TOKEN_TYPE::RECONSIDER},
        {"reduce", TOKEN_TYPE::REDUCE},
        {"reducibility", TOKEN_TYPE::REDUCIBILITY},
        {"redefine", TOKEN_TYPE::REDEFINE},
        {"reflexivity", TOKEN_TYPE::REFLEXIVITY},
        {"registration", TOKEN_TYPE::REGISTRATION},
        {"registrations", TOKEN_TYPE::REGISTRATIONS},
        {"requirements", TOKEN_TYPE::REQUIREMENTS},
        {"reserve", TOKEN_TYPE::RESERVE},
        {"sch", TOKEN_TYPE::SCH},
        {"scheme", TOKEN_TYPE::SCHEME},
        {"schemes", TOKEN_TYPE::SCHEMES},
        {"section", TOKEN_TYPE::SECTION},
        {"selector", TOKEN_TYPE::SELECTOR},
        {"set", TOKEN_TYPE::SET},
        {"sethood", TOKEN_TYPE::SETHOOD},
        {"st", TOKEN_TYPE::ST},
        {"struct", TOKEN_TYPE::STRUCT},
        {"such", TOKEN_TYPE::SUCH},
        {"suppose", TOKEN_TYPE::SUPPOSE},
        {"symmetry", TOKEN_TYPE::SYMMETRY},
        {"synonym", TOKEN_TYPE::SYNONYM},
        {"take", TOKEN_TYPE::TAKE},
        {"that", TOKEN_TYPE::THAT},
        {"the", TOKEN_TYPE::THE},
        {"then", TOKEN_TYPE::THEN},
        {"theorem", TOKEN_TYPE::THEOREM},
        {"theorems", TOKEN_TYPE::THEOREMS},
        {"thesis", TOKEN_TYPE::THESIS},
        {"thus", TOKEN_TYPE::THUS},
        {"to", TOKEN_TYPE::TO},
        {"transitivity", TOKEN_TYPE::TRANSITIVITY},
        {"uniqueness", TOKEN_TYPE::UNIQUENESS},
        {"vocabularies", TOKEN_TYPE::VOCABULARIES},
        {"when", TOKEN_TYPE::WHEN},
        {"where", TOKEN_TYPE::WHERE},
        {"with", TOKEN_TYPE::WITH},
        {"wrt", TOKEN_TYPE::WRT},
    };

    auto it = keyword_map.find(text);
    if (it != keyword_map.end()) {
        return it->second;
    } else {
        return TOKEN_TYPE::UNKNOWN;
    }
}

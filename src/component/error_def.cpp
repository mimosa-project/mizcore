#include <cassert>

#include "error_def.hpp"

using mizcore::ERROR_LEVEL;
using mizcore::ERROR_TYPE;

std::string_view
mizcore::GetErrorMessage(ERROR_TYPE error)
{
    switch (error) {
        case ERROR_TYPE::SUCCESS:
            return "Success.";
        case ERROR_TYPE::UNKNOWN:
            return "Unknown error occured.";
        case ERROR_TYPE::BLOCK_NOT_CLOSED_IN_ARTICLE:
            return "Block must be closed by the end of the article.";
        case ERROR_TYPE::BLOCK_NOT_IN_SECTION:
            return "Block must be located after begin keyword.";
        case ERROR_TYPE::CASE_BLOCK_NOT_UNDER_PROOF_BLOCK:
            return "Case block must be located under proof block.";
        case ERROR_TYPE::END_KEYWORD_WITH_NO_PAIR_BLOCK:
            return "The end keyword must be used in pairs with a block "
                   "keyword.";
        case ERROR_TYPE::END_KEYWORD_WITHOUT_SEMICOLON:
            return "The end keyword must be followed by a semicolon.";
        case ERROR_TYPE::ENVIRON_KEYWORD_AFTER_BEGIN_KEYWORD:
            return "The environ keyword must be located before begin keyword.";
        case ERROR_TYPE::HEREBY_BLOCK_NOT_UNDER_PROOF_BLOCK:
            return "Hereby block must be located under proof block.";
        case ERROR_TYPE::NOW_BLOCK_NOT_UNDER_PROOF_BLOCK:
            return "Now block must be located under proof block.";
        case ERROR_TYPE::PROOF_START_WITHOUT_PROPOSITION:
            return "Proof block must not start without preceding proposition.";
        case ERROR_TYPE::STATEMENT_IS_EMPTY:
            return "Statement must not be empty.";
        case ERROR_TYPE::STATEMENT_NOT_CLOSED_IN_ARTICLE:
            return "Statement must be closed by the end of the article.";
        case ERROR_TYPE::SUPPOSE_BLOCK_NOT_UNDER_PROOF_BLOCK:
            return "Suppose block must be located under proof block.";
        case ERROR_TYPE::TOKEN_IS_UNKNOWN:
            return "Unknown token found.";
        default:
            return "Unknown error message";
    }
}

int
mizcore::GetErrorCode(ERROR_TYPE error)
{
    return int(error);
}

ERROR_LEVEL
mizcore::GetErrorLevel(ERROR_TYPE error)
{
    switch (error) {
        case ERROR_TYPE::STATEMENT_IS_EMPTY:
            return ERROR_LEVEL::WARNING;
        default:
            return ERROR_LEVEL::ERROR;
    }
}

std::string_view
mizcore::GetErrorLevelText(ERROR_LEVEL level)
{
    switch (level) {
        case ERROR_LEVEL::ERROR:
            return "ERROR";
        case ERROR_LEVEL::WARNING:
            return "WARNING";
        case ERROR_LEVEL::INFO:
            return "INFO";
        default:
            return "UNKNOWN ERROR LEVEL";
    }
}

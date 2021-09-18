#pragma once

#include <string_view>

namespace mizcore {

enum class ERROR_TYPE
{
    SUCCESS = 0,
    UNKNOWN = 999,

    // Block parser errors
    BLOCK_NOT_CLOSED_IN_ARTICLE,
    END_KEYWORD_WITH_NO_PAIR_BLOCK,
    END_KEYWORD_WITHOUT_SEMICOLON,
    ENVIRON_KEYWORD_APPEARED_AFTER_BEGIN_KEYWORD,
    PROOF_START_WITHOUT_PROPOSITION,
    STATEMENT_IS_EMPTY,
    STATEMENT_NOT_CLOSED_IN_ARTICLE,
    TOKEN_IS_UNKNOWN,
};

enum class ERROR_LEVEL
{
    ERROR,
    WARNING,
    INFO,
};

std::string_view
GetErrorMessage(ERROR_TYPE error);

int
GetErrorCode(ERROR_TYPE error);

ERROR_LEVEL
GetErrorLevel(ERROR_TYPE error);

std::string_view
GetErrorLevelText(ERROR_LEVEL level);

} // namespace mizcore

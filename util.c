#include "util.h"

bool
cca_util_is_alpha(char c)
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool
cca_util_is_numeric(char c)
{
    return c >= '0' && c <= '9';
}

bool
cca_util_is_alphanumeric(char c)
{
    return cca_util_is_alpha(c) || cca_util_is_numeric(c);
}

bool
cca_util_is_whitespace(char c)
{
    return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\v';
}

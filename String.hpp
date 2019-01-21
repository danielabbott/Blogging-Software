#pragma once

#include <string>
#include <algorithm>
#include <cctype>

static inline void make_lowercase(std::string & s)
{
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
}

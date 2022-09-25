#include "Utils/StringUtils.h"

#include <algorithm>
#include <sstream>
#include <vector>

std::vector<std::string> StringUtils::Split(const std::string& Str, const char Delimeter)
{
    std::vector<std::string> OutStrings;

    std::istringstream f(Str);
    std::string s;

    while (getline(f, s, Delimeter))
    {
        OutStrings.push_back(s);
    }

    return OutStrings;
}

std::string StringUtils::Join(const std::vector<std::string>& StrArray, const char Delimeter)
{
    std::string Ret;

    for (size_t i = 0; i < StrArray.size(); ++i)
    {
        Ret += StrArray[i] + Delimeter;
    }

    Ret.pop_back();
    return Ret;
}

std::string StringUtils::Replace(const std::string& Str, char From, char To)
{
    std::string s = Str;
    std::replace(s.begin(), s.end(), From, To);
    return s;
}

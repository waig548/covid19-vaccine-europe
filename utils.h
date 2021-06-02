//
// Created by waig548 on 5/31/2021.
//

#ifndef COVID_UTILS_H
#define COVID_UTILS_H

#include <vector>
#include <regex>

static std::vector<std::string> split(std::string s, const std::string& delimiter)
{
    size_t pos = 0;
    std::vector<std::string> tmp;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        tmp.push_back(s.substr(0, pos));
        s.erase(0, pos+delimiter.length());
    }
    tmp.push_back(s);
    return tmp;
}

static size_t stoullOrElse(const std::string& s, size_t defaultValue = 0)
{
    if (s.empty())
        return defaultValue;
    return std::stoull(s);
}

static std::vector<size_t> parseYearWeekISO(const std::string& s)
{
    if (!std::regex_match(s, std::regex("[0-9]+-W[0-9]+")))
        throw std::exception("Malformed input");
    std::vector<size_t> tmp;
    for (const auto& e :split(s, "-W"))
        tmp.push_back(stoullOrElse(e));
    return tmp;
}

static std::string toLowerCase(const std::string& s)
{
    std::string tmp;
    std::transform(s.begin(), s.end(), tmp.begin(), [](unsigned char c){return tolower(c);});
    return tmp;
}

static bool equalsCaseInsensitive(const std::string& a, const std::string& b)
{
    return toLowerCase(a)==toLowerCase(b);
}

#endif //COVID_UTILS_H

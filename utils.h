//
// Created by waig548 on 5/31/2021.
//

#ifndef COVID_UTILS_H
#define COVID_UTILS_H

#include <vector>

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

#endif //COVID_UTILS_H

//
// Created by waig548 on 6/1/2021.
//

#include <string>
#include <vector>
#include <functional>
#include "utils.h"

using std::function;
using std::string;

#ifndef COVID_OPERATOR_H
#define COVID_OPERATOR_H

template <typename T>
class Operator
{
public:
    string key;
    //bool numericOnly;
    std::function<bool(T, T)> check;

    static const Operator<size_t> eq;
    static const Operator<size_t> neq;
    static const Operator<size_t> lt;
    static const Operator<size_t> leq;
    static const Operator<size_t> gt;
    static const Operator<size_t> geq;
    static const Operator<size_t> unknown;

    static const Operator<string> eqs;
    static const Operator<string> neqs;
    static const Operator<string> unknowns;

    static Operator<T> of(const string& k);
    static std::vector<Operator<T>> values();
    bool operator== (const Operator<T>& b);
private:
    size_t ordinal;
    static std::vector<Operator<size_t>> numericOps;
    static std::vector<Operator<string>> stringOps;
    static size_t ordinals;
    //template<typename U>
    Operator(string k, const std::function<bool(size_t , size_t)>& lambda, size_t o = ordinals++);
    Operator(string k, const std::function<bool(string, string)>& lambda, size_t o = ordinals++);
};




#endif //COVID_OPERATOR_H

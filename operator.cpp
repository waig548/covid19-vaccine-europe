//
// Created by waig548 on 6/1/2021.
//

#include "operator.h"

#include <utility>

template<> std::vector<Operator<size_t>> Operator<size_t>::numericOps = std::vector<Operator<size_t>>();
template<> std::vector<Operator<string>> Operator<string>::stringOps = std::vector<Operator<string>>();


template<> const Operator<size_t> Operator<size_t>::eq = Operator("==",  [](size_t a, size_t b)->bool { return a == b; });
template<> const Operator<size_t> Operator<size_t>::neq = Operator("!=", [](size_t a, size_t b)->bool {return a!=b;});
template<> const Operator<size_t> Operator<size_t>::lt = Operator("<", [](size_t a, size_t b)->bool {return a<b;});
template<> const Operator<size_t> Operator<size_t>::leq = Operator("<=", [](size_t a, size_t b)->bool {return a<=b;});
template<> const Operator<size_t> Operator<size_t>::gt = Operator(">", [](size_t a, size_t b)->bool {return a>b;});
template<> const Operator<size_t> Operator<size_t>::geq = Operator(">=", [](size_t a, size_t b)->bool {return a>=b;});
template<> const Operator<size_t> Operator<size_t>::unknown = Operator("ereoreereoroeooeor", [](size_t a, size_t b)->bool {return false;});

template<> const Operator<string> Operator<string>::eqs = Operator<string>("==", [](const string& a, const string& b)->bool {return a == b;});
template<> const Operator<string> Operator<string>::neqs = Operator("!=", [](const string& a, const string& b)->bool {return a != b;});
template<> const Operator<string> Operator<string>::unknowns = Operator("erroroeoroeoroeoroe", [](const string& a, const string& b)->bool {return false;});

template<typename T>
Operator<size_t> Operator<T>::ofNumericOps(const string& k)
{
    for (auto o : numericOps)
        if (o.key == k)
            return o;
    return unknown;
}

template<typename T>
std::vector<Operator<size_t>> Operator<T>::numericOperators()
{
    auto tmp = numericOps;
    tmp.pop_back();
    return tmp;
}

template<typename T>
Operator<string> Operator<T>::ofStringOps(const string& k)
{
    for (auto o : stringOps)
        if (o.key == k)
            return o;
    return unknowns;
}

template<typename T>
std::vector<Operator<string>> Operator<T>::stringOperators()
{
    auto tmp = stringOps;
    tmp.pop_back();
    return tmp;
}

template<> Operator<size_t> Operator<size_t>::of(const string& k)
{
    for (auto o : numericOps)
        if (o.key == k)
            return o;
    return unknown;
}

template<> Operator<string> Operator<string>::of(const string& k)
{
    for (auto o : stringOps)
        if (o.key == k)
            return o;
    return unknowns;
}

template<> std::vector<Operator<size_t>> Operator<size_t>::values()
{
    auto tmp = numericOps;
    tmp.pop_back();
    return tmp;
}

template<> std::vector<Operator<string>> Operator<string>::values()
{
    auto tmp = stringOps;
    tmp.pop_back();
    return tmp;
}

template<typename T>
Operator<T>::Operator(string k, const function<bool(size_t, size_t)> &lambda): key(std::move(k)), check(lambda)
{
    numericOps.push_back(*this);
}

template<typename T>
Operator<T>::Operator(string k, const function<bool(string, string)> &lambda): key(std::move(k)), check(lambda)
{
    stringOps.push_back(*this);
}


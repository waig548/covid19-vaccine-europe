//
// Created by waig548 on 6/1/2021.
//

#include <string>
#include <vector>
#include <functional>

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

    //template<typename U>
    static Operator<T> of(const string& k);

    //template<typename U>
    static std::vector<Operator<T>> values();

    static Operator<size_t> ofNumericOps(const string& k);
    static std::vector<Operator<size_t>> numericOperators();
    static Operator<string> ofStringOps(const string& k);
    static std::vector<Operator<string>> stringOperators();
private:
    static std::vector<Operator<size_t>> numericOps;
    static std::vector<Operator<string>> stringOps;

    //template<typename U>
    Operator(string k, const std::function<bool(size_t , size_t)>& lambda);


    Operator(string k, const std::function<bool(string, string)>& lambda);     /*
    Operator<size_t>(string k, const std::function<bool(size_t, size_t)>& lambda): key(std::move(k)), check(lambda)
    {
        numericOps.push_back(*this);
    }
    Operator<string>(string k, const std::function<bool(string, string)>& lambda): key(std::move(k)), check(lambda)
    {
        stringOps.push_back(*this);
    }
    */

};




#endif //COVID_OPERATOR_H

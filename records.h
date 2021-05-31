//
// Created by waig548 on 5/25/2021.
//
#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include "utils.h"

using std::string;
using std::vector;
using std::map;
using std::ifstream;

#ifndef COVID_RECORDS_H
#define COVID_RECORDS_H

class Record
{
private:
public:
    map<string, string> stringValues;
    map<string, size_t> numericValues;

    explicit Record(vector<string> data);
    bool operator==(const Record& other);
};

class DataSet
{
public:
    explicit DataSet(ifstream& file);

    vector<Record> records;
};



#endif //COVID_RECORDS_H

//
// Created by waig548 on 5/25/2021.
//

#include "records.h"

using std::getline;

Record::Record(vector<string> data)
{
    vector<size_t> t = parseYearWeekISO(data[0]);
    numericValues["year"] = t[0];
    numericValues["week"] = t[1];
    numericValues["first"] = stoullOrElse(data[1]);
    numericValues["refused"] = stoullOrElse(data[2]);
    numericValues["second"] = stoullOrElse(data[3]);
    numericValues["unknown"] = stoullOrElse(data[4]);
    numericValues["received"] = stoullOrElse(data[5]);
    stringValues["region"] = data[6];
    numericValues["population"] = stoullOrElse(data[7]);
    stringValues["country"] = data[8];
    stringValues["target"] = data[9];
    stringValues["vaccine"] = data[10];
    numericValues["denominator"] = stoullOrElse(data[11]);
}

bool Record::operator==(const Record & other)
{
    for(auto & numericValue : numericValues)
        if (numericValue.second != other.numericValues.at(numericValue.first))
            return false;
    for (auto & stringValue : stringValues)
        if (stringValue.second != other.stringValues.at(stringValue.first))
            return false;
    return true;
}

DataSet::DataSet(ifstream& file)
{
    // skip the first line
    string line;
    getline(file, line);
    while (getline(file, line))
    {
        vector<string> l = split(line, string(","));
        Record record(l);
        records.push_back(record);
    }
}

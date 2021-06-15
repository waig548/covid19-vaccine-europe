//
// Created by waig548 on 5/31/2021.
//

#include "field.h"

#include <utility>

std::vector<Field> Field::valueList = std::vector<Field>();
size_t Field::ordinals = 0;

const Field Field::Year = Field("Year", "year", 0, Numeric);
const Field Field::Week = Field("Week", "week", 0, Numeric);
const Field Field::YearWeekISO = Field("YearWeekISO", "yearweekiso", 0, String);
const Field Field::FirstDose = Field("FirstDose", "first", 1, Numeric);
const Field Field::FirstDoseRefused = Field("FirstDoseRefused", "refused", 2, Numeric);
const Field Field::SecondDose = Field("SecondDose", "second", 3, Numeric);
const Field Field::UnknownDose = Field("UnknownDose", "unknown", 4, Numeric);
const Field Field::NumberDosesReceived = Field("NumberDosesReceived", "received", 5, Numeric);
const Field Field::Region = Field("Region", "region", 6);
const Field Field::Population = Field("Population", "population", 7, Numeric);
const Field Field::ReportingCountry = Field("ReportingCountry", "country", 8);
const Field Field::TargetGroup = Field("TargetGroup", "target", 9);
const Field Field::Vaccine = Field("Vaccine", "vaccine", 10);
const Field Field::Denominator = Field("Denominator", "denominator", 11, Numeric);

const Field Field::Unknown = Field("Unknown", "errorerroereroeofoe", -1);

Field Field::of(const string &k)
{
    for (auto e : valueList)
        if (equalsCaseInsensitive(e.key, k))
            return e;
    return Unknown;
}

std::vector<Field> Field::values()
{
    auto tmp = valueList;
    tmp.pop_back();
    return tmp;
}

Field::Field(string n, string k, size_t i, Type t, size_t o): name(std::move(n)), key(std::move(k)), index(i), type(t), ordinal(o)
{
    valueList.push_back(*this);
}

bool operator==(const Field& l, const Field& r)
{
    return l.ordinal==r.ordinal;
}

bool operator!=(const Field& l, const Field& r)
{
    return !(l==r);
}

bool operator<(const Field& l, const Field& r)
{
    return l.ordinal<r.ordinal;
    if (l==Field::Year && r==Field::Week)
        return true;
    if (l.index<r.index)
        return true;
    return false;
}




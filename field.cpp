//
// Created by waig548 on 5/31/2021.
//

#include "field.h"

#include <utility>

std::vector<Field> Field::valueList = std::vector<Field>();

const Field Field::Year = Field("year", 0, Numeric);
const Field Field::Week = Field("week", 0, Numeric);
const Field Field::FirstDose = Field("first", 1, Numeric);
const Field Field::FirstDoseRefused = Field("refused", 2, Numeric);
const Field Field::SecondDose = Field("second", 3, Numeric);
const Field Field::UnknownDose = Field("unknown", 4, Numeric);
const Field Field::NumberDosesReceived = Field("received", 5, Numeric);
const Field Field::Region = Field("region", 6);
const Field Field::Population = Field("population", 7, Numeric);
const Field Field::ReportingCountry = Field("country", 8);
const Field Field::TargetGroup = Field("target", 9);
const Field Field::Vaccine = Field("vaccine", 10);
const Field Field::Denominator = Field("denominator", 11, Numeric);

const Field Field::Unknown = Field("errorerroereroeofoe", -1);

Field Field::of(const string &k)
{

    for (auto e : valueList)
        if (e.key == k)
            return e;
    return Unknown;
}

std::vector<Field> Field::values()
{
    return valueList;
}

Field::Field(string k, size_t i, Type t): key(std::move(k)), index(i), type(t)
{
    valueList.push_back(*this);
}



//
// Created by waig548 on 5/31/2021.
//

#include <string>
#include <map>
#include <vector>
#include "utils.h"

using std::string;

#ifndef COVID_FIELD_H
#define COVID_FIELD_H

class Field final
{
public:
    enum Type: size_t{Numeric = 0, String = 1};
    string key;
    size_t index;
    Type type;
    string name;
    static const Field Year;
    static const Field Week;
    static const Field YearWeekISO;
	static const Field FirstDose;
	static const Field FirstDoseRefused;
	static const Field SecondDose;
	static const Field UnknownDose;
	static const Field NumberDosesReceived;
	static const Field Region;
	static const Field Population;
	static const Field ReportingCountry;
	static const Field TargetGroup;
	static const Field Vaccine;
	static const Field Denominator;
	static const Field Unknown;

    static Field of(const string& k);
    static std::vector<Field> values();

    friend bool operator== (const Field& l, const Field& r);
    friend bool operator!= (const Field& l, const Field& r);
    friend bool operator< (const Field& l, const Field& r);

private:
    size_t const ordinal;
    static std::vector<Field> valueList;
    static size_t ordinals;
    Field(string n, string k, size_t i, Type t = String, size_t o = ordinals++);
};

#endif //COVID_FIELD_H

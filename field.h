//
// Created by waig548 on 5/31/2021.
//

#include <string>
#include <map>
#include <vector>

using std::string;

#ifndef COVID_FIELD_H
#define COVID_FIELD_H

class Field final
{
public:
    enum Type {String, Numeric};
    string key;
    size_t index;
    Type type;
    static const Field Year;
    static const Field Week;
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
    static std::vector<Field> valueList;

    Field(string  k, size_t i, Type t = String);
};

#endif //COVID_FIELD_H

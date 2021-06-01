//
// Created by waig548 on 6/1/2021.
//

#include "data_frame.h"

DataFrame::DataFrame(ifstream &file, const vector<streampos>& lines) : fileStream(file), indices(lines)
{
    curLine = 0;
    curPos = lines[curLine];
    fileStream.seekg(curPos);
}

DataFrame DataFrame::copy()
{
    return DataFrame(fileStream, indices);
}

DataFrame DataFrame::copy(const vector<streampos>& newIndices)
{
    return DataFrame(fileStream, newIndices);
}

DataFrame &DataFrame::goToLine(size_t line)
{
    curLine=line;
    curPos=indices[curLine];
    fileStream.seekg(curPos);
    return *this;
}

DataFrame::Entry DataFrame::readEntry()
{
    string raw;
    getline(fileStream, raw);
    curLine++;
    curPos=indices[curLine];
    if (curPos==indices.back())
    {
        curLine = 0;
        curPos=indices.front();
        if (fileStream.eof())
            fileStream.clear();
        fileStream.seekg(curPos);
    }
    return Entry(raw);
}

DataFrame::Entry::Entry(const vector<string>& data)
{
    auto s=data.size();
    for (const auto& f : Field::values())
    {
        if (f==Field::Year)
            nums[f]=parseYearWeekISO(data[f.index])[0];
        else if (f==Field::Week)
            nums[f]=parseYearWeekISO(data[f.index])[1];
        else if (f.type==Field::Type::Numeric)
            nums[f]=stoullOrElse(data[f.index]);
        else if (f.type==Field::Type::String)
            strings[f]=data[f.index];
        else
            throw std::exception("should not happen");
    }
}

DataFrame::Entry::Entry(const string& rawEntry) : Entry(split(rawEntry, ","))
{

}

size_t DataFrame::Entry::getNumeric(Field &field)
{
    if (field.type!=Field::Numeric)
        throw std::exception("invalid type");
    return nums[field];
}

string DataFrame::Entry::getString(Field &field)
{
    if (field.type!=Field::String)
        throw std::exception("invalid type");
    return strings[field];
}
//
// Created by waig548 on 6/1/2021.
//

#include "data_frame.h"

DataFrame::DataFrame(ifstream &file, const vector<streampos>& lines) : fileStream(file), indices(lines)
{
    //curLine = 0;
    curPos = fileStream.tellg();
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

DataFrame &DataFrame::goTo(streampos pos)
{
    //curLine=std::find(indices.begin(), indices.end(), pos) - indices.begin();
    curPos=pos;
    fileStream.seekg(curPos);
    return *this;
}

DataFrame &DataFrame::goToLine(size_t line)
{
    //curLine=line;
    curPos=indices[line];
    fileStream.seekg(curPos);
    return *this;
}

DataFrame::Entry DataFrame::readEntry()
{
    string raw;
    getline(fileStream, raw);
    //curLine++;
    curPos=fileStream.tellg();
    if (curPos==indices.back())
    {
        //curLine = 0;
        curPos=indices.front();
        if (fileStream.eof())
            fileStream.clear();
        fileStream.seekg(curPos);
    }
    return Entry(raw);
}

vector<streampos> DataFrame::getIndices() const
{
    return indices;
}

size_t DataFrame::getCurrentLine() const
{
    return -1;//curLine;
}

streampos DataFrame::getCurrentPos() const
{
    return curPos;
}


DataFrame::Entry::Entry(const vector<string>& data)
{
    auto s=data.size();
    for (const auto& f : Field::values()) {
        if (f == Field::Year)
            nums[f] = parseYearWeekISO(data[f.index])[0];
        else if (f == Field::Week)
            nums[f] = parseYearWeekISO(data[f.index])[1];
        else if (f.type == Field::Type::Numeric)
            nums[f] = stoullOrElse(data[f.index]);
        else if (f.type == Field::Type::String) {
            string tmp = data[f.index];
            //std::transform(tmp.begin(), tmp.end(), tmp.begin(), [](unsigned char c){return tolower(c);});
            strings[f] = data[f.index];//tmp;
        } else
            throw std::exception("should not happen");
        if (f.type == Field::Numeric)
        {
            values[f] = nums[f];
            vaa[f] = std::make_any<size_t>(nums[f]);
        }
        else
        {
            values[f] = strings[f];
            vaa[f] = std::make_any<string>(strings[f]);
        }
    }
}

DataFrame::Entry::Entry(const string& rawEntry) : Entry(split(rawEntry, ","))
{

}

size_t DataFrame::Entry::getNumeric(const Field& field)
{
    if (field.type!=Field::Numeric)
        throw std::exception("invalid type");
    return nums[field];
}

string DataFrame::Entry::getString(const Field& field)
{
    if (field.type!=Field::String)
        throw std::exception("invalid type");
    return strings[field];
}

std::variant<size_t, string> DataFrame::Entry::getu(const Field& field)
{
    return values[field];
}

std::any DataFrame::Entry::geta(const Field &field)
{
    return std::any();
}

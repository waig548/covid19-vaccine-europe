//
// Created by waig548 on 6/1/2021.
//

#include <fstream>
#include <unordered_map>
#include <variant>
#include <any>
#include "field.h"
#include "operator.h"
#include "utils.h"

using std::ifstream;
using std::vector;
using std::streampos;
using std::map;
using std::unordered_map;
using std::pair;

#ifndef COVID_DATA_FRAME_H
#define COVID_DATA_FRAME_H

class DataFrame
{
    ifstream &fileStream;
    vector<streampos> indices;
    //size_t curLine;
    streampos curPos;
public:
    class Entry
    {
        friend DataFrame;
        map<Field, size_t> nums;
        map<Field, string> strings;
        //map<Field, pair<size_t, string>> values;
        //map<Field, data> values;
        map<Field, std::variant<size_t, string>> values;
        map<Field, std::any> vaa;
    protected:
        explicit Entry(const vector<string>& data);
        explicit Entry(const string& rawEntry);
    public:
        size_t getNumeric(const Field& field);
        string getString(const Field& field);
        //pair<size_t, string> get(Field &field);
        std::variant<size_t, string> getu(const Field& field);
        std::any geta(const Field& field);
    };

    DataFrame(ifstream &file, const vector<streampos>& lines);
    DataFrame copy();
    DataFrame copy(const vector<streampos>& newIndices);

    DataFrame& goTo(streampos pos);
    DataFrame& goToLine(size_t line);
    Entry readEntry();
    vector<streampos> getIndices() const;
    size_t getCurrentLine() const;
    streampos getCurrentPos() const;
};

#endif //COVID_DATA_FRAME_H

#pragma comment(lib, "urlmon.lib")
#pragma once

#include <iostream>
#include <string>
#include <urlmon.h>
#include "data_frame.h"

using std::cin;
using std::cout;
using std::endl;
using std::find;
using std::ifstream;
using std::vector;
using std::string;

static const char url[] = "https://opendata.ecdc.europa.eu/covid19/vaccine_tracker/csv/data.csv";
static const char filepath[] = "data.csv";

bool updateData()
{
    cout << "Retrieving latest data..." << endl;
    return SUCCEEDED(URLDownloadToFile(nullptr, url, filepath, 0, nullptr));
}

bool loadData(ifstream &file)
{
    file.open(filepath, std::ios_base::in);
    return file.good();

}

bool processData(ifstream &file, vector<streampos> &indices)
{
    cout << "Processing data..." << endl;
    string line;
    getline(file, line);
    //auto tt = file.tellg();
    do
        indices.push_back(file.tellg());
    while (getline(file, line));
    file.clear();
    file.seekg(indices[0]);
    //auto tt=file.tellg();
    indices.pop_back();
    return true;
}

void printHelp()
{

}

bool processQuery(string &query, DataFrame &data, vector<streampos> &selected)
{
    cout << "Processing query..." << endl;
    vector<string> args = split(query, " ");

    if (args[0] == "include" || args[0] == "exclude" || args[0] == "filter" )
    {
        bool add = args[0] == "include";
        bool neg = args[0] == "exclude";
        auto f = Field::of(args[1]);
        if (f==Field::Unknown)
            return false;
        if (f.type==Field::Numeric)
        {
            auto op=Operator<size_t>::of(args[2]);
            auto value=stoullOrElse(args[3]);
            if (add)
                for (auto pos: data.getIndices())
                {
                    if (std::count(selected.begin(),selected.end(), pos))
                        continue;
                    if (op.check(data.goTo(pos).readEntry().getNumeric(f), value))
                        selected.push_back(pos);
                }
            else
            {
                vector<streampos> tmp;
                for (auto pos: selected)
                    if (neg ^ op.check(data.goTo(pos).readEntry().getNumeric(f), value))
                        tmp.push_back(pos);
                selected = tmp;
            }
        }
        else
        {
            auto op=Operator<string>::of(args[2]);
            auto value=args[3];
            if (add)
                for (auto pos: data.getIndices())
                {
                    if (std::count(selected.begin(),selected.end(), pos))
                        continue;
                    if (op.check(data.goTo(pos).readEntry().getString(f), value))
                        selected.push_back(pos);
                }
            else
            {
                vector<streampos> tmp;
                for (auto pos: selected)
                    if (op.check(data.goTo(pos).readEntry().getString(f), value))
                        tmp.push_back(pos);
                selected = tmp;
            }
        }
        cout << "Query successful." << endl;
        cout << "Selected: " << selected.size() << " entries." << endl;
    }
    else if (args[0] == "analyze")
    {

    }
    else
        return false;


    return true;
}

int main()
{
    if(updateData())
        cout << "Success! File stored at " << filepath << endl;
    else
        cout << "Failed, fallback to local data at " << filepath << endl;

    ifstream file;

    if(!loadData(file))
    {
        cout << "Unable to read the file " << filepath << endl << "Exiting..." << endl;
        exit(1);
    }

    vector<streampos> indices;

    if (!processData(file, indices))
        exit(1);
    cout << "Data processing complete." << endl;
    cout << "Total: " << indices.size() << " entries." << endl;
    DataFrame data(file, indices);
    vector<streampos> selected;
    cout << "Enter help for query instructions" << endl;
    while (true)
    {
        cout << ">";
        string query;
        getline(cin, query);
        if (query == "exit")
            break;
        if (query == "help")
            printHelp();
        else if (!processQuery(query, data, selected))
            cout << "Query error, please check your input." << endl;
    }
    cout << "Exiting..." << endl;

    return 0;
}
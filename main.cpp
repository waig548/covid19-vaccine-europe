#pragma comment(lib, "urlmon.lib")
#pragma once

#include <iostream>
#include <string>
#include <urlmon.h>
#include <iomanip>
#include "data_frame.h"

using std::cin;
using std::cout;
using std::endl;
using std::find;
using std::ifstream;
using std::vector;
using std::string;
using std::pair;

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

bool processQuery(string &query, DataFrame &data, vector<streampos> &selected, vector<pair<string, vector<streampos>>> &stored, map<string, vector<streampos>> &group)
{
    auto o = Operator<size_t >::eq;
    auto s = Operator<string >::eqs;

    cout << "Processing query..." << endl;
    vector<string> args = split(query, " ");
    args[0] = toLowerCase(args[0]);

    if (args.size() == 2 && equalsCaseInsensitive(args[0], "include") && equalsCaseInsensitive(args[1], "all"))
    {
        selected=data.getIndices();
        cout << "Query successful." << endl;
        cout << "Selected: " << selected.size() << " entries." << endl;
    }
    else if (args[0] == "include" || args[0] == "exclude" || args[0] == "filter" )
    {
        if ((args.size()-1)%3)
            return false;
        bool add = args[0] == "include";
        bool neg = args[0] == "exclude";
        size_t it=0;
        size_t conditions = (args.size()-1)/3;
        while (it<conditions)
        {

            args[it*3+1] = toLowerCase(args[it*3+1]);
            auto f = Field::of(args[it*3+1]);
            if (f==Field::Unknown)
                return false;
            if (f.type==Field::Numeric)
            {
                auto op = Operator<size_t>::of(args[it*3+2]);
                if (op == (Operator<size_t>::unknown))
                    return false;
                auto value = stoullOrElse(args[it*3+3]);
                if (add && it == 0)
                {
                    vector<streampos> tmp;
                    for (auto pos: data.getIndices())
                    {
                        if (std::count(selected.begin(), selected.end(), pos))
                            continue;
                        if (op.check(data.goTo(pos).readEntry().getNumeric(f), value))
                            tmp.push_back(pos);
                    }
                    for (auto d: tmp)
                        if (!std::count(selected.begin(), selected.end(), d))
                            selected.push_back(d);
                    std::sort(selected.begin(), selected.end());
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
                auto op=Operator<string>::of(args[it*3+2]);
                if (op == Operator<string>::unknowns)
                    return false;
                auto value=args[it*3+3];
                if (add && it == 0)
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
                        if (neg ^ op.check(data.goTo(pos).readEntry().getString(f), value))
                            tmp.push_back(pos);
                    selected = tmp;
                }
            }
            it++;
        }
        cout << "Query successful." << endl;
        cout << "Selected: " << selected.size() << " entries." << endl;
    }
    else if (args[0] == "group")
    {
        if (args.size()!=2)
            return false;
        auto field = Field::of(args[1]);
        if (field==Field::Unknown)
            return false;
        if (field.type!=Field::String)
            return false;
        group.clear();
        for (auto p : selected)
        {
            auto e = data.goTo(p).readEntry();
            if (group.find(e.getString(field))==group.end())
                group[e.getString(field)]=vector<streampos>();
            group[e.getString(field)].push_back(p);
        }
        cout << "Query successful." << endl;
        cout << "Groups built from selected entries: " << group.size() << " groups." << endl;
    }
    else if (args[0] == "store")
    {

    }
    else if (args[0] == "load")
    {

    }
    else if (args[0] == "show")
    {
        if (args.size() < 2)
            return false;
        if (equalsCaseInsensitive(args[1], "group"))
        {
            if (args.size()==2)
            {
                cout << "groups:" << endl;
                for (const auto& e : group)
                    cout << "key: " << e.first << ", size: " << e.second.size() << endl;
            }
            else
            {
                if (group.find(args[2])==group.end())
                    return false;
                cout << "Selected group key: " << args[2] << endl;
                if (group[args[2]].size()>20)
                    cout << "Showing first 20 entries:" << endl;
                cout << std::setw(4) << "index";
                for (const auto& f : Field::values())
                    cout << std::setw(12) << f.key;
                cout << endl;
                for (size_t i = 0; i < min(group[args[2]].size(), 20); ++i)
                {
                    cout << std::setw(4) << i;
                    auto entry = data.goTo(group[args[2]][i]).readEntry();
                    for (auto f : Field::values())
                    {
                        cout << std::setw(12);
                        if (f.type==Field::Numeric)
                            cout << entry.getNumeric(f);
                        else if (f.type==Field::String)
                            cout << entry.getString(f);
                    }
                    cout << endl;
                }
                if (group[args[2]].size()>20)
                    cout << "..." << endl;
            }
        }
        else if (equalsCaseInsensitive(args[1], "selected"))
        {
            if (selected.size()>20)
                cout << "Showing first 20 entries:" << endl;
            cout << std::setw(4) << "index";
            for (const auto& f : Field::values())
                cout << std::setw(12) << f.key;
            cout << endl;
            for (size_t i = 0; i < min(selected.size(), 20); ++i)
            {
                cout << std::setw(4) << i;
                auto entry = data.goTo(selected[i]).readEntry();
                for (auto f : Field::values())
                {
                    cout << std::setw(12);
                    if (f.type==Field::Numeric)
                        cout << entry.getNumeric(f);
                    else if (f.type==Field::String)
                        cout << entry.getString(f);
                }
                cout << endl;
            }
            if (selected.size()>20)
                cout << "..." << endl;
        }
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
    vector<pair<string, vector<streampos>>> storedSelection;
    map<string, vector<streampos>> group;
    cout << "Enter help for query instructions" << endl;
    while (true)
    {
        cout << ">";
        string query;
        getline(cin, query);
        //std::transform(query.begin(), query.end(), query.begin(), [](unsigned char c){return tolower(c);});
        if (query == "exit")
            break;
        if (query == "help")
            printHelp();
        else if (!processQuery(query, data, selected, storedSelection, group))
            cout << "Query error, please check your input." << endl;
    }
    cout << "Exiting..." << endl;

    return 0;
}
#pragma comment(lib, "urlmon.lib")
#pragma once

#include <iostream>
#include <utility>
#include <set>
#include <urlmon.h>
#include "records.h"

using std::cin;
using std::cout;
using std::endl;
using std::find;

static vector<string> vars = {"year", "week", "country", "received", "first", "refused", "second", "unknown", "region", "target", "vaccine", "population", "denominator"};
static vector<string> nnvars = {"country", "region", "target", "vaccine"};
static vector<string> ops = {"<", "<=", ">", ">=", "="};

void printHelp()
{
    cout << "Query example: year = 2020 and country = AT" << endl;
    cout << R"(Description: get a set of records that the year of the reports is "2020" and the country code of the country reported is "AT")" << endl << endl;
    cout << "Available variables:" << endl <<
         "Name\t\tType\t\tDescription" << endl <<
         "year\t\tNumeric\t\tthe year of report" << endl <<
         "week\t\tNumeric\t\tthe week of report" << endl <<
         "country\t\tString\t\tcountry code of the reporting country" << endl <<
         "received\tNumeric\t\tNumber of doses received" << endl <<
         "first\t\tNumeric\t\tNumber of first dose vaccine administered" << endl <<
         "refused\t\tNumeric\t\tNumber of first dose vaccine refused" << endl <<
         "second\t\tNumeric\t\tNumber of second dose vaccine administered" << endl <<
         "unknown\t\tNumeric\t\tNumber of doses administered where the type was not specified" << endl <<
         "region\t\tString\t\tRegion code of the specified region" << endl <<
         "target\t\tString\t\tTargeted Group" << endl <<
         "vaccine\t\tString\t\tName of vaccine" << endl <<
         "population\tNumeric\t\tAge-specific population for the country" << endl <<
         "denominator\tNumeric\t\tPopulation denominators for target groups" << endl << endl <<
         "For further details,\nreference: https://www.ecdc.europa.eu/sites/default/files/documents/Variable_Dictionary_VaccineTracker-03-2021.pdf" << endl;
    cout << "Available operators:" << endl <<
         "Relational: <, <=, >, >=, =" << endl <<
         "Logical: and, or" << endl <<
         "All relational operators except \"=\" are only applicable to numeric variables." << endl <<
         "All symbols in the query must be separated by a space." << endl;
}

size_t processQuery(const string& query, const DataSet& data, vector<Record>& result)
{
    vector<string> parameters = split(query, " ");
    string log;
    size_t logicalCount = 0;
    size_t i = 0;
    vector<Record> tmp;
    while(i<parameters.size())
    {
        if (parameters[i] == "and" || parameters[i] == "or")
        {
            log = parameters[i];
            i++;
            continue;
        }
        string var, op, val;
        /*for (int j = 0; j < 3; ++j)
        {
            if ((i-logicalCount)%3==0)
            {*/
                if (find(vars.begin(), vars.end(), parameters[i]) == vars.end())
                {
                    cout << "Invalid variable at index " << i << endl;
                    return 1;
                }
                var = parameters[i];
                i++;
        /*    }
            else if ((i-logicalCount)%3==1)
            {*/
                if (find(ops.begin(), ops.end(), parameters[i]) == ops.end())
                {
                    cout << "Invalid operator at index " << i << endl;
                    return 1;
                }
                op = parameters[i];
                i++;
        /*    }
            else
            {*/
                val = parameters[i];
                i++;
            //}
        //}
        bool isNumericVar = find(nnvars.begin(), nnvars.end(), var) == nnvars.end();
        if (!isNumericVar && op != "=")
        {
            cout << "Invalid use of operator " << op << " on variable " << var << endl;
            return 1;
        }
        const vector<Record> *ptr;
        if (tmp.empty() || log == "or")
            ptr=&(data.records);
        else
            ptr=&result;
        if (isNumericVar)
        {
            size_t value=std::stoull(val);
            for (auto rec : *ptr) {
                if (op == "=")
                {    if (rec.numericValues[var] == value)
                        tmp.push_back(rec);
                }
                else if(op == ">")
                {    if (rec.numericValues[var] > value)
                        tmp.push_back(rec);
                }
                else if(op == ">=")
                {    if (rec.numericValues[var] >= value)
                        tmp.push_back(rec);
                }
                else if(op == "<")
                {    if (rec.numericValues[var] < value)
                        tmp.push_back(rec);
                }
                else if(op == "<=")
                    if (rec.numericValues[var] <= value)
                        tmp.push_back(rec);
            }
        }
        else
        {
            for (auto rec : *ptr)
                if (rec.stringValues[var] == val)
                    tmp.push_back(rec);
        }
        if (log=="or")
        {    for (const auto& rec : tmp)
                if (find(result.begin(), result.end(), rec) == result.end())
                    result.push_back(rec);
        }
        else
            result.assign(tmp.begin(), tmp.end());
    }
    return 0;
}


int main()
{
    const char url[] = "https://opendata.ecdc.europa.eu/covid19/vaccine_tracker/csv/data.csv";
    const char filepath[] = "data.csv";
    cout << "Retrieving latest data..." << endl;
    if(SUCCEEDED(URLDownloadToFile(nullptr, url, filepath, 0, nullptr)))
        cout << "Success! File stored at " << filepath << endl;
    else
        cout << "Failed, fallback to local data at " << filepath << endl;

    ifstream file;
    file.open(filepath, std::ios_base::in);
    if(!file.good())
    {
        cout << "Unable to read the file " << filepath << endl << "Exiting..." << endl;
        exit(1);
    }
    cout << "Processing data..." << endl;
    DataSet data(file);
    file.close();
    cout << "Total record count: "<< data.records.size() << endl;
    cout << "Enter help for a list of query instructions." << endl;
    while(true)
    {
        cout << ">";
        string query;
        std::getline(cin, query);
        if (query == "exit")
        {
            cout << "Exiting..." << endl;
            break;
        }
        else if (query == "help")
            printHelp();
        else
        {
            vector<Record> result;
            if(processQuery(query, data, result))
                cout << "Query error";
            cout << "Query result size" << result.size() << endl;
        }
    }

    return 0;
}

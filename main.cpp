#pragma comment(lib, "urlmon.lib")
#pragma once

#include <iostream>
#include <utility>
//#include <vector>
#include <string>
#include <urlmon.h>
#include <fstream>
#include <unordered_map>
#include "field.h"
#include "utils.h"
//#include "records.h"

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

bool processData(ifstream &file, vector<size_t> &indices)
{
    cout << "Processing data..." << endl;
    string line;
    //getline(file, line);
    auto tt=file.tellg();
    while (getline(file, line))
    {
        tt=file.tellg();
        indices.push_back(tt);
    }
    return true;
}

void printHelp()
{

}

bool processQuery(string &query)
{
    vector<string> args = split(query, " ");

    if (args[0] == "filter")
    {

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

    vector<size_t> indices;

    if (!processData(file, indices))
        exit(1);
    cout << "Data processing complete." << endl;
    cout << "Total: " << indices.size() << " entries." << endl;
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
        else if (!processQuery(query))
            cout << "Query error, please check your input." << endl;
    }
    cout << "Exiting..." << endl;
    return 0;
}
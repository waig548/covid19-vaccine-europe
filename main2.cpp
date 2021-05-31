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

bool processData(ifstream file, vector<size_t> &indices)
{
    string line;
    //getline(file, line);
    while (getline(file, line))
    {
        indices.push_back(SEEK_CUR);
    }
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

    cout << "Processing data..." << endl;
    cout << 
}
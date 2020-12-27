#ifndef EX4_TIMESERIES_H
#define EX4_TIMESERIES_H

#include <map>
#include <vector>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

typedef map<string, float> featuresAndValues;

class TimeSeries {

private:
    const char* fileName;
    string line, colName, token;
    int index;
    float val;

public:

    map<int, string> columnFeature;
    vector<map<string, float>> data;

    TimeSeries(const char* CSVfileName) {
        this->fileName = CSVfileName;
        saveData(fileName);
    }

    // saves the data from the csv file.
    void saveData(const char* fileName);

    // return a vector of values of a certain feature.
    vector<float> getValues(string featureName) const;

    // destructor.
    ~TimeSeries();
};


#endif //EX4_TIMESERIES_H

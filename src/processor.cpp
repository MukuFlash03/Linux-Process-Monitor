#include "processor.h"
#include "linux_parser.h"

#include <bits/stdc++.h>
#include <string>
#include <vector> 

using std::vector;
using std::string;
using std::stoi;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    vector<long> prevVals(10,0);
    vector<long> currVals;
    double cpuPercent{0.0};

    vector<string> timeValues = LinuxParser::CpuUtilization();

    for (auto i : timeValues)
        currVals.emplace_back(stoi(i));

    long currTotal = std::accumulate(currVals.begin(), currVals.end(), 0);
    long prevTotal = std::accumulate(prevVals.begin(), prevVals.end(), 0);

    long currIdle = currVals[3] + currVals[4];
    long prevIdle = prevVals[3] + prevVals[4];

    long netTotal = currTotal - prevTotal;
    long netIdle = currIdle - prevIdle;

    cpuPercent = (netTotal - netIdle) / netTotal;

    for (int i = 0; i < 10; i++)
        prevVals[i] = currVals[i];

    return cpuPercent;
}
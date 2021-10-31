#include <iostream>
#include <vector>
#include <string>
#include <bits/stdc++.h>

using namespace std;

int main() {
    vector<int>prevVals(10,1);
    for (auto i : prevVals)
        cout << i << '\n';

    long prevTotal = std::accumulate(prevVals.begin(), prevVals.end(), 0);
    cout << prevTotal << '\n';

    double cpuPercent{0.0};
    cout << cpuPercent << '\n';
    return 0;
}
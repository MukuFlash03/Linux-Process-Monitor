#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using namespace std;

/*
using std::set;
using std::size_t;
using std::string;
using std::vector;
*/

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
    UpdateProcesses();
    return processes_;
}

// TODO: Return the system's kernel identifier (string)
string System::Kernel() { return LinuxParser::Kernel(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// TODO: Return the operating system name
string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }

void System::UpdateProcesses() {
    processes_.clear();
    for (int pid : LinuxParser::Pids())
        UpdateProcess(pid);
    sort(processes_.begin(), processes_.end());
}

void System::UpdateProcess(int pid) {
    // Process process(pid);
    processes_.emplace_back(pid);
}

/* Suggestion by Code Reviewer
vector < Process > & System::Processes() {
    const vector < int > & pids = LinuxParser::Pids();
    for (const int & pid: pids) {
        processes_.emplace_back(pid);
    }
    return processes_;
}
*/
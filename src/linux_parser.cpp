#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <boost/lexical_cast.hpp>
#include <bits/stdc++.h>
#include <unistd.h>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// Self-developed support function to fetch specific parameter value as seen in file/cmd line result
std::string LinuxParser::getFileCMDParameterValue(std::string path, std::string parameter) {
  string line;
  string key, value;
  std::ifstream filestream(path);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == parameter)
        return value;
    }
  }
  return {}; 
}

std::string LinuxParser::getFileCMDParameterValue(std::string path, unsigned long position) {
  string line, property, desc;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    using StrIt = std::istream_iterator<std::string>;
    std::vector<std::string> container{StrIt{linestream}, StrIt{}};
    if (container.size() >= position + 1) {
      return container.at(position);
    }
  }
  return {};
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// Alternate Method
// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string memName, memT, memF;
  float fracUsedMem{0.0};
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line); // Line 1 -> MemTotal
    std::istringstream linestream1(line);
    linestream1 >> memName >> memT;

    std::getline(filestream, line); // Line 2 -> MemFree
    std::istringstream linestream2(line);
    linestream2 >> memName >> memF;

  // String to Float conversions and Used memory calculation
    float memTotal = boost::lexical_cast<float>(memT);
    float memFree = boost::lexical_cast<float>(memF);
    float usedMem = memTotal - memFree;
    fracUsedMem = usedMem/memTotal;
    return fracUsedMem;
  }
  return fracUsedMem;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line;
  string upT, idleT;
  long sysUpTime{0};

  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> upT >> idleT;

    sysUpTime = stol(upT);
    return sysUpTime;
  }
  return sysUpTime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  string key, value;
  vector<string> values;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key; // "cpu" keyword extracted

    while (linestream >> value)
      values.emplace_back(value);

   return values;
  }
  return values;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string value = getFileCMDParameterValue(kProcDirectory + kStatFilename, "processes");
  int numProcess{0};
  
  if (!value.empty()) {
    numProcess = boost::lexical_cast<int>(value);
    return numProcess;
  }
  return numProcess; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string value = getFileCMDParameterValue(kProcDirectory + kStatFilename, "procs_running");
  int numProcess{0};
  
  if (!value.empty()) {
    numProcess = boost::lexical_cast<int>(value);
    return numProcess;
  }
  return numProcess; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, cmd);
    return cmd;
  }
  return {};
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
  string value = getFileCMDParameterValue(kProcDirectory + to_string(pid) + kStatusFilename, "VmSize:");
  float memProcess{0.0};
  
  if (!value.empty()) {
    memProcess = stof(value);
    memProcess /= 1024;
    return to_string(memProcess);
  }
  return to_string(memProcess);
}

// TODO: Read and return the CPU usage used by a process
float LinuxParser::CpuUtilization(int pid) {
  string path = kProcDirectory + to_string(pid) + kStatFilename;

  long sysUpTime = UpTime();
  long utime = stol(getFileCMDParameterValue(path, 13)); // 13th index element in cmdresult or file token index
  long stime = stol(getFileCMDParameterValue(path, 14)); // 14th index element in cmdresult or file token index
  long cutime = stol(getFileCMDParameterValue(path, 15)); // 15th index element in cmdresult or file token index
  long cstime = stol(getFileCMDParameterValue(path, 16)); // 16th index element in cmdresult or file token index  
  long starttime = stol(getFileCMDParameterValue(path, 21)); // 21st index element in cmdresult or file token index

  long hertz = sysconf(_SC_CLK_TCK);

  long totalTime = utime + stime;
  totalTime += cutime + cstime; // include time from child processes

  float secTime = sysUpTime - (starttime / hertz);
  float cpuUsage = 100 * (totalTime / hertz) / secTime;
  
  return cpuUsage;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string uid = getFileCMDParameterValue(kProcDirectory + to_string(pid) + kStatusFilename, "Uid:");
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string line;
  string key, value, x;
  string uid = Uid(pid);

  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (value.compare(uid) == 0) {
          return key;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
  string path = kProcDirectory + to_string(pid) + kStatFilename;
  string value = getFileCMDParameterValue(path, 21); // 21st index element in cmdresult or file token index

  long secVal{0};

  if (!value.empty()) {
    long clkTicks = stol(value);
    long hertz = sysconf(_SC_CLK_TCK);
    secVal = clkTicks / hertz;
    return secVal;
  }
  return secVal;
}
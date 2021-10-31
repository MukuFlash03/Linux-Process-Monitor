/*

YOU CAN ALSO DEFINE SOME GENERIC FUNCTIONS AS WELL AS UTILITY FUNCTIONS WHICH WILL HELP YOU MINIMISE REPETITIVE CODE !
Here is an example:

template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &filename) {
  std::string line, key;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == keyFilter) {
          return value;
        }
      }
    }
  }
  return value;
};

template <typename T>
T getValueOfFile(std::string const &filename) {
  std::string line;
  T value;

  std::ifstream stream(kProcDirectory + filename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
};
Usage of the above template to get the value:

Use of findValueByKey

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string memTotal = "MemTotal:";
  string memFree = "MemFree:";
  float Total = findValueByKey<float>(memTotal, kMeminfoFilename);// "/proc/memInfo"
  float Free = findValueByKey<float>(memFree, kMeminfoFilename);
  return (Total - Free) / Total;
}
Use of getValueOfFile

string LinuxParser::Command(int pid) {
  return std::string(getValueOfFile<std::string>(std::to_string(pid) + kCmdlineFilename));
}

*/
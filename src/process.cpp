#include "process.h"
#include "linux_parser.h"

#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {
  cpu_util = CpuUtilization();
}

// DONE: Return this process's ID
int Process::Pid() {  return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {  
  using namespace LinuxParser;
  string line, key, value;

  long uptime_secs = LinuxParser::UpTime();
  float cpu_utilization = 0.0;

  std::ifstream stream(kProcDirectory + to_string(pid_) + kStatFilename);

  int utime = 0, 
      stime = 0, 
      cutime = 0, 
      cstime = 0;
  float starttime = 0.0;

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), '(', '_');
      std::replace(line.begin(), line.end(), ')', '_');
      std::replace(line.begin(), line.end(), '-', '_');
      std::istringstream linestream(line);
      std::istream_iterator<std::string> beg(linestream), end;
      std::vector<std::string> vec(beg, end);
      
      utime = std::stoi(vec[13]);
      stime = std::stoi(vec[14]);
      cutime = std::stoi(vec[15]);
      cstime = std::stoi(vec[16]);
      starttime = std::stof(vec[21]);

      return ((float)(utime + stime + cutime + cstime)) / (uptime_secs * sysconf(_SC_CLK_TCK) - starttime);
    }
  }
  return cpu_utilization;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
  return (this->cpu_util < a.cpu_util); 
}

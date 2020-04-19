#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

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
  string os, version, kernel;
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float memfree = 0.0, memtotal = 0.0;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while (std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value){
        if (key == "MemTotal"){
          memtotal = stof(value);
        }
        if(key =="MemFree"){
          memfree = stof(value);
        }
      }
    }
  }
  return (memtotal - memfree) / memtotal;
}


// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string line, value;
  long uptime = 0;
  std::ifstream stream("/proc/uptime");
  if (stream.is_open()){    
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> value;
    return stol(value);
  }
  return uptime;
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  return {}; 
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, total_procs, key;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> total_procs) {
        if (key == "processes"){
          return std::stoi(total_procs);
        }
      }
    }
  }
  return 0; 
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {   
  return LinuxParser::Pids().size(); 
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      return line;
    }
  }
  return string(); 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  int ram;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize"){
          ram = stoi(value)/1024; 
        }
      }
    }
  }
  return to_string(ram); 
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, uid, key;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()){
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid"){
          return uid;
        }
      }
    }
  }
  return string(); 
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line, uid, _uid, placeholder, user;
  std::ifstream pid_stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::ifstream username_stream(kPasswordPath);
  if (pid_stream.is_open() && username_stream.is_open()){
    uid = Uid(pid);
    while (std::getline(username_stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> placeholder >> _uid) {
        if (_uid == uid){
          return user;
        }
      }
    }
  }
  return string(); 
}

// DONE: Read and return the uptime of a process in seconds
long LinuxParser::UpTime(int pid) {
  string line, key, value;
  long clock = 0;
  
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), '(', '_');
      std::replace(line.begin(), line.end(), ')', '_');
      std::replace(line.begin(), line.end(), '-', '_');
      std::istringstream linestream(line);
      std::istream_iterator<std::string> beg(linestream), end;
      std::vector<std::string> vec(beg, end);
      clock = std::stol(vec[21]);
    }
  }

  return (clock / sysconf(_SC_CLK_TCK));
}

#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
using std::stof;
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
  vector<int> process_ids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int process_id = stoi(filename);
        process_ids.push_back(process_id);
      }
    }
  }
  closedir(directory);
  return process_ids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float total, free;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
      std::remove(line.begin(), line.end(), ' ');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> key;
      if(key == "MemTotal") {
        linestream >> total;
      } else if (key == "MemFree") {
        linestream >> free;
      }
    }
  }
  return (total-free)/total; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }

  return stol(uptime); 
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  return 0;
  }

// TODO: Read and return the number of active jiffies for a process_id
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int process_id[[maybe_unused]]) { 


  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  
  return 0;
  }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  
  return 0;
  }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, value;
  vector<string> cpu_utilisation;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu;
    while (linestream >> value) {
      cpu_utilisation.emplace_back(value);
    } 
  }
  return cpu_utilisation; 
  }

float LinuxParser::CpuUtilization(int process_id) { 
  string line, value; 
  int i = 0;
  vector<string> cpu_utilisation;
  std::ifstream stream(kProcDirectory + std::to_string(process_id) + kStatFilename);
  if(stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> value && i < 23) {
      cpu_utilisation.emplace_back(value);
      i++;
    }
  }
  float total_time = stof(cpu_utilisation[13]) + stof(cpu_utilisation[14]) + stof(cpu_utilisation[15]) + stof(cpu_utilisation[16]);
  float seconds = LinuxParser::UpTime() - (stof(cpu_utilisation[21])/sysconf(_SC_CLK_TCK) );
  return (total_time/sysconf(_SC_CLK_TCK)/seconds);
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if(key == "processes") {
        return stoi(value);
      }
    }
    }
  }
  return stoi(value); 
  }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, key, value;
  int running_processes;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key;
      if(key == "procs_running") {
        linestream >> running_processes;
      }
    }
  }
  return running_processes; 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int process_id) { 
  string value;
  std::ifstream stream(kProcDirectory + to_string(process_id) + kCmdlineFilename);
  if(stream.is_open()) {
    std::getline(stream, value);
  }
  return value; 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int process_id) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(process_id) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> key >> value;
      if(key == "VmSize:") {
        return std::to_string(stol(value)/1000);
      }
    }
  }
  return value;
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int process_id) { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + to_string(process_id) + kStatusFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    linestream >> key;
      if(key == "Uid") {
        linestream >> value;
        return value;
      }
    
    }
  }
  return value; 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int process_id) { 
  string line, key, value, link;
  string uid = Uid(process_id);
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
   linestream >> key >> link >> value;
      if(value == uid) {
        return key;
      }
    }
    }
  return value; 
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int process_id) { 
  string line, value;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(process_id) + kStatFilename);
  if(stream.is_open()) {
    while(std::getline(stream, line)) {
    std::istringstream linestream(line);
    while (linestream >> value) {
      values.emplace_back(value);
    }
    }
  }
  return (stol(values[21])/sysconf(_SC_CLK_TCK)); 
  }

#include "processor.h"
#include "linux_parser.h"
#include <string>
#include <vector>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<long> cpu_utilization{};
    for (auto pid : LinuxParser::CpuUtilization()) {
        cpu_utilization.emplace_back(std::stol(pid));
    }
    float total_jiffies, idle_jiffies;
    total_jiffies = cpu_utilization[LinuxParser::kUser_] + cpu_utilization[LinuxParser::kNice_] + cpu_utilization[LinuxParser::kSystem_] + cpu_utilization[LinuxParser::kIdle_] + cpu_utilization[LinuxParser::kIOwait_] + cpu_utilization[LinuxParser::kIRQ_] + cpu_utilization[LinuxParser::kSoftIRQ_] + cpu_utilization[LinuxParser::kSteal_];
    idle_jiffies = cpu_utilization[LinuxParser::kIdle_] + cpu_utilization[LinuxParser::kIOwait_];
    return (total_jiffies - idle_jiffies) / total_jiffies;
    }
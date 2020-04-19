#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  using namespace LinuxParser;
  using std::string;
  
  string line, key;
  float cpu_util = 0.0;

  string user = "", 
         nice = "", 
         system= "", 
         idle = "", 
         iowait = "",
         irq = "", 
         softirq = "", 
         steal = "";
  
  std::ifstream stream(kProcDirectory + kStatFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        if (key == "cpu") {
          float user_f = std::stof(user);
          float nice_f = std::stof(nice);
          float system_f = std::stof(system);
          float idle_f = std::stof(idle);
          float iowait_f = std::stof(iowait);
          float irq_f = std::stof(irq);
          float softirq_f = std::stof(softirq);
          float steal_f = std::stof(steal);

          float idle = idle_f + iowait_f;
          float non_idle = user_f + nice_f + system_f + irq_f + softirq_f + steal_f;
          float total = idle + non_idle;
          float prev_total = prev_idle_ + prev_non_idle_;

          float delta_total = total - prev_total;
          float delta_idle = idle - prev_idle_;

          cpu_util = (delta_total - delta_idle) / delta_total;

          prev_idle_ = idle;
          prev_non_idle_ = non_idle;
        }
      }
    }
  }
  return cpu_util;
}

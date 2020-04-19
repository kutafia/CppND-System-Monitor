#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float prev_idle_ = 0.0;
  float prev_non_idle_ = 0.0;
};

#endif

#include "format.h"

#include <string>
#include <iostream>  
#include <math.h>  

using std::string;
using std::to_string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string hours = "", minutes = "", secs = "";

  hours = to_string(seconds / 3600);
  hours.length() == 1 ? hours = "0" + hours : hours = hours;
  
  minutes = to_string((seconds % 3600) / 60);
  minutes.length() == 1 ? minutes = "0" + minutes : minutes = minutes;
  
  secs = to_string(seconds % 60);
  secs.length() == 1 ?  secs = "0" + secs : secs = secs;   

  return hours + ":" + minutes + ":" + secs;
}

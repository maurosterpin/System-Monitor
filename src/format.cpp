#include <string>

#include "format.h"

using std::string;
using std::to_string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long s) { 
    string hours, minutes, seconds;
    int h = s / 3600;
    if(h < 10) {hours = "0" + to_string(h);}
    else {hours = to_string(h);}
    int m = s % 3600 / 60;
    if(m < 10) {minutes = "0" + to_string(m);}
    else {minutes = to_string(m);}
    s = s % 3600 % 60;
    if(s < 10) {seconds = "0" + to_string(s);}
    else {seconds = to_string(s);}
    return hours + ":" + minutes + ":" + seconds; 
    }
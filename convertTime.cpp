#include "initialData.h"



std::string convertTime(time_t time) {
  std::tm *localTime = std::localtime(&time);
  // Create a buffer to store the formatted time string
  char timeBuffer[80];
  // Format the time string
  std::strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", localTime);
  return std::string(timeBuffer);
}
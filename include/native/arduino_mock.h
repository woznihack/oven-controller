#include <chrono>
#include <iostream>
#include <ArduinoFake.h>

// Returns the number of milliseconds since program start
unsigned long millis() {
  using namespace std::chrono;
  return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

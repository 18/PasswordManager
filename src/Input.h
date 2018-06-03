#pragma once
#include <map>
#include <chrono>

class Input
{
public:
  static bool GetKeyDown(char key, unsigned delay = 150);
private:
  static std::map<char, std::chrono::time_point<std::chrono::steady_clock>> keys_;
};
/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-10-02 12:36:52  
 * @Description:
 */

#pragma once

#include <chrono>

class Timer {
  using clock = std::chrono::high_resolution_clock;
  clock::time_point _start;

 public:
  Timer() { start(); }
  ~Timer() {}
  void start() { _start = clock::now(); }
  double elapsed() const {
    return std::chrono::duration<double>(clock::now() - _start).count();
  }
};
/* 
 * @Author: Zhijie Cai  
 * @Date: 2022-10-02 12:35:01  
 * @Description: 
 */

#pragma once

#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "utils/timer.hpp"

namespace info {

bool show_log = true;

namespace Color {
const std::string BLACK("\033[30m");
const std::string RED("\033[31m");
const std::string GREEN("\033[32m");
const std::string YELLOW("\033[33m");
const std::string BLUE("\033[34m");
const std::string MAGENTA("\033[35m");
const std::string CYAN("\033[36m");
const std::string WHITE("\033[37m");
}  // namespace Color

enum class LogType {
  kError = 0,
  kWarning = 1,
  kInfo = 2,
  kSummary = 3,
  kDebug = 4
};

std::string log_type_color(LogType type) {
  if (type == LogType::kError) return Color::RED;
  if (type == LogType::kWarning) return Color::YELLOW;
  if (type == LogType::kInfo) return Color::GREEN;
  if (type == LogType::kSummary) return Color::CYAN;
  if (type == LogType::kDebug) return Color::MAGENTA;
  return Color::WHITE;
}

std::string log_type_name(LogType type) {
  if (type == LogType::kError) return "ERROR";
  if (type == LogType::kWarning) return "WARNNIG";
  if (type == LogType::kInfo) return "INFO";
  if (type == LogType::kSummary) return "SUMMARY";
  if (type == LogType::kDebug) return "DEBUG";
  return "?????";
}

std::vector<Timer> clocks;
std::vector<std::string> phase_name;

void write(LogType type, std::string s) {
  if (show_log == false) return;
  if (type == LogType::kInfo) {
    std::cerr << log_type_color(type) << s << std::endl << Color::WHITE;
  } else {
    std::cerr << log_type_color(type) << "<" << log_type_name(type) << ">"
              << " " << s << std::endl
              << Color::WHITE;
  }
}

void error(std::string s) { write(LogType::kError, s); }
void warning(std::string s) { write(LogType::kWarning, s); }
void info(std::string s) { write(LogType::kInfo, s); }
void summary(std::string s) { write(LogType::kSummary, s); }

void __add_clock(std::string name) {
  clocks.emplace_back();
  phase_name.emplace_back(name);
}

std::string __delete_clock() {
  double elapsed = clocks.back().elapsed();
  std::stringstream ss;
  std::string s;
  ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << elapsed;
  ss >> s;
  clocks.pop_back();
  phase_name.pop_back();
  return s;
}

void start(std::string usage = "", std::string append = "") {
  if (usage == "") usage = "temporary clock";
  if (append != "" && append[0] != ' ') append = " " + append;
  info(usage + append + " ...");
  __add_clock(usage);
}

void finish(std::string append = "") {
  if (clocks.empty() || phase_name.empty()) {
    error("clock's start and end did not match");
  } else {
    if (append != "" && append[0] != ' ') append = " " + append;
    std::string log = phase_name.back() + append;
    log = "[" + __delete_clock() + "s] " + log + " finish.";
    info(log);
  }
}

void phase(const std::string& phase_name, std::function<void(void)> f) {
  start(phase_name);
  f();
  finish();
}

}  // end namespace log
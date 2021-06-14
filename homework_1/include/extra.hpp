#ifndef EXTRA_H_
#define EXTRA_H_

#include <string>
#include <vector>

constexpr const char* reset_color_ = "\033[39m";

inline std::string Red(std::string s) noexcept {
  s = "\033[31m" + s + reset_color_;
  return s;
}

inline std::string Green(std::string s) noexcept {
  s = "\033[32m" + s + reset_color_;
  return s;
}

inline std::string Blue(std::string s) noexcept {
  s = "\033[34m" + s + reset_color_;
  return s;
}

inline std::string Bold(std::string s) noexcept {
  s = "\033[1m" + s + "\033[22m";
  return s;
}

inline std::vector<std::string> Split(const std::string& s, const std::string& sep) {
  std::vector<std::string> v;
  size_t previous = 0;
  size_t current = 0;
  while ((current = s.find(sep, current)) != std::string::npos) {
    v.push_back(s.substr(previous, current - previous));
    current += sep.size();
    previous = current;
  }
  v.push_back(s.substr(previous));
  return v;
}

#endif  // EXTRA_H_

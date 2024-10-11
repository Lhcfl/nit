#include "./nit_colors.hpp"
#include "./nit_types.hpp"
#include "config/config.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#ifndef LINCA_NIT_LOGGER
#define LINCA_NIT_LOGGER

using NitProjectConfig::NIT_LOG_LEVELS;

class NitLogger {

  std::string header, spliter;
  NIT_LOG_LEVELS log_level = NIT_LOG_LEVELS::DEBUG;

  inline bool level_unsatisified(NIT_LOG_LEVELS level) const {
    return log_level > level || NitProjectConfig::NIT_LOG_LEVEL > level;
  }

  template <typename T> void output_one(const T &something) const {
    std::cout << something;
  }

  template <typename T>
  typename std::enable_if<NitTypeUtils::has_begin_end<T>::value>::type
  output_one(const T &arr) const {
    std::cout << "[";
    int isBegin = true;
    for (auto &item : arr) {
      if (!isBegin)
        std::cout << ", ";
      output_one(item);
      isBegin = false;
    }
    std::cout << "]";
  }

  void output_one(const std::string &str) const { std::cout << str; }
  void output_one(const std::filesystem::directory_iterator &di) const {
    std::cout << "[Files] {\n";
    for (const auto &item : di) {
      std::cout << "- " << item.path().filename().string() << "\n";
    }
    std::cout << "}";
  }

  template <typename T> void output(const T &something) const {
    output_one(something);
    std::cout << std::endl;
  }
  template <typename T, typename... Rargs>
  void output(const T &something, const Rargs &...other) const {
    output_one(something);
    std::cout << spliter;
    output(other...);
  }

public:
  explicit NitLogger(const std::string &header = "",
                     const std::string &spliter = " ")
      : spliter(spliter) {
    if (header.size() > 0) {
      this->header = "[" + header + "] ";
    }
  }

  NitLogger &changeHeader(const std::string &header) {
    this->header = header;
    return *this;
  }
  NitLogger &changeLevel(NIT_LOG_LEVELS level) {
    this->log_level = level;
    return *this;
  }
  void disable() { this->log_level = NIT_LOG_LEVELS::DISABLED; }
  void enable() { this->log_level = NIT_LOG_LEVELS::DEBUG; }

  template <typename... Args> void log(Args... args) const {
    std::cout << header;
    output(args...);
  }
  template <typename... Args> void debug(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::DEBUG))
      return;
    std::cout << NitColor::white("[DEBUG] ") << header;
    output(args...);
  }
  template <typename... Args> void info(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::INFO))
      return;
    std::cout << "[INFO] " << header;
    output(args...);
  }
  template <typename... Args> void warn(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::WARN))
      return;
    std::cout << NitColor::yellow("[WARN] ") << header;
    output(args...);
  }
  template <typename... Args> void error(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::ERROR))
      return;
    std::cout << NitColor::red("[ERROR] ") << header;
    output(args...);
  }
  template <typename... Args> void success(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::ERROR))
      return;
    std::cout << NitColor::green("[SUCCESS] ") << header;
    output(args...);
  }
};

#endif
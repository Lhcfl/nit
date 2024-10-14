#include "./nit_colors.hpp"
#include "./nit_types.hpp"
#include "config/config.h"
#include <filesystem>
#include <iostream>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>
// #include <vector>

#ifndef LINCA_NIT_LOGGER
#define LINCA_NIT_LOGGER

using NitProjectConfig::NIT_LOG_LEVELS;

class NitLogger {
  class NoEndLineClass {};

  std::stringstream _sStream;
  std::ostream &oStream;
  bool isSStream = false;

  std::string header, spliter;
  NIT_LOG_LEVELS log_level = NIT_LOG_LEVELS::DEBUG;

  inline bool level_unsatisified(NIT_LOG_LEVELS level) const {
    return log_level > level || NitProjectConfig::NIT_LOG_LEVEL > level;
  }

  template <typename T>
  typename std::enable_if<!NitTypeUtils::has_begin_end<T>::value>::type
  output_one(const T &something) const {
    oStream << something;
  }

  template <typename T>
  typename std::enable_if<NitTypeUtils::has_begin_end<T>::value>::type
  output_one(const T &arr) const {
    oStream << "[";
    int isBegin = true;
    for (auto &item : arr) {
      if (!isBegin)
        oStream << ", ";
      output_one(item);
      isBegin = false;
    }
    oStream << "]";
  }

  void output_one(const std::string &str) const { oStream << str; }
  void output_one(const std::string_view &str) const { oStream << str; }
  void output_one(const std::filesystem::directory_iterator &di) const {
    oStream << "[Files] {\n";
    for (const auto &item : di) {
      oStream << "- " << item.path().filename().string() << "\n";
    }
    oStream << "}";
  }

  void output(const NoEndLineClass &something) const {
    // do nothing
  }
  template <typename T> void output(const T &something) const {
    output_one(something);
    oStream << std::endl;
  }
  template <typename T, typename... Rargs>
  void output(const T &something, const Rargs &...other) const {
    output_one(something);
    oStream << spliter;
    output(other...);
  }

public:
  static const NoEndLineClass NO_ENDL;
  explicit NitLogger(const std::string &header = "",
                     const std::string &spliter = " ",
                     std::ostream &outputer = std::cout)
      : spliter(spliter), oStream(outputer) {
    if (header.size() > 0) {
      this->header = "[" + header + "] ";
    }
  }
  explicit NitLogger(const std::string &header, const std::string &spliter,
                     const std::string &init_of_stringstream)
      : spliter(spliter), _sStream(init_of_stringstream), oStream(_sStream),
        isSStream(true) {
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

  /**
   * get stringstream
   */
  std::stringstream &getStream() {
    if (!isSStream) {
      throw std::invalid_argument(
          "You should only get a stream of a stringstream NitLogger");
    }
    return _sStream;
  }

  void newLine() { oStream << std::endl; }
  template <typename... Args> void log(Args... args) const {
    oStream << header;
    output(args...);
  }
  template <typename... Args> void debug(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::DEBUG))
      return;
    oStream << NitColor::white("[DEBUG] ") << header;
    output(args...);
  }
  template <typename... Args> void info(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::INFO))
      return;
    oStream << "[INFO] " << header;
    output(args...);
  }
  template <typename... Args> void warn(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::WARN))
      return;
    oStream << NitColor::yellow("[WARN] ") << header;
    output(args...);
  }
  template <typename... Args> void error(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::ERROR))
      return;
    oStream << NitColor::red("[ERROR] ") << header;
    output(args...);
  }
  template <typename... Args> void success(Args... args) const {
    if (level_unsatisified(NIT_LOG_LEVELS::ERROR))
      return;
    oStream << NitColor::green("[SUCCESS] ") << header;
    output(args...);
  }
};

#endif
#include "./nit_error.hpp"
#include "./utils.hpp"
#include "apis/apis.h"
#include <fstream>
#include <string>

#pragma once

#include <filesystem>
namespace NitFs {

class NitFsError : public NitError {
public:
  NitFsError(const std::string &message) : NitError(message) {}
};

auto inline readDir(const std::string &dir = UsefulApi::cwd()) {
  return std::filesystem::directory_iterator(UsefulApi::cwd());
}

bool inline hasFile(const std::string &filename,
                    const std::string &dir = UsefulApi::cwd()) {
  return readDir() |
         Utils::Some<std::filesystem::directory_entry>(
             [filename](const std::filesystem::directory_entry &item) {
               return item.path().filename().string() == filename;
             });
}

inline std::string dirRelative(const std::string &s) {
  return UsefulApi::cwd() + "/" + s;
}

/**
 * 助教代码的修改 - 使用 const std::string& 改善了传参性能，使用 std::exception
 * 抛出异常
 */
inline void writeToFile(const std::string &filePath, const std::string &str) {
  // 检查并创建目录
  std::filesystem::path path(filePath);
  if (!std::filesystem::exists(path.parent_path())) {
    std::filesystem::create_directories(path.parent_path());
  }
  std::ofstream file(filePath, std::ios_base::out | std::ios_base::binary);
  bool is_open = file.is_open();
  if (!is_open) {
    throw NitFsError("Cannot open file " + filePath);
  }
  file << str;
  file.close();
}

inline std::string readFromFile(const std::string &filePath) {
  std::ifstream file(filePath, std::ios_base::in | std::ios_base::binary);
  bool is_open = file.is_open();
  if (!is_open) {
    throw NitFsError("Cannot open file " + filePath);
  }
  std::string str(static_cast<std::istreambuf_iterator<char>>(file),
                  std::istreambuf_iterator<char>());
  file.close();
  return str;
}

} // namespace NitFs
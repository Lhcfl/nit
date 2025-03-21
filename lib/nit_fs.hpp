#include "./nit_error.hpp"
#include "./utils.hpp"
#include "apis/apis.h"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

#ifndef LINCA_NIT_FS
#define LINCA_NIT_FS
namespace NitFs {

class NitFsError : public NitError {
public:
  NitFsError(const std::string &message) : NitError(message) {}
  NitFsError(IdentifiableId id, const std::string &message)
      : NitError(id, message) {}
};

using UsefulApi::cwd;

auto inline readDir(const std::string &dir = UsefulApi::cwd()) {
  return std::filesystem::directory_iterator(UsefulApi::cwd());
}

inline std::vector<std::string>
listFiles(const std::string &dir = UsefulApi::cwd()) {
  return UsefulApi::listFilesInDirectory(dir);
}

bool inline hasFile(const std::string &filename,
                    const std::string &dir = UsefulApi::cwd()) {
  return readDir() |
         Utils::Some<std::filesystem::directory_entry>(
             [filename](const std::filesystem::directory_entry &item) {
               return item.path().filename().string() == filename;
             });
}

bool inline existsFile(const std::string &filename) {
  return std::filesystem::exists(filename);
}

inline std::string dirAbsolute(const std::string &s) {
  return UsefulApi::cwd() + "/" + s;
}
inline std::string fileIn(const std::string &path,
                          const std::string &filename) {
  if (path.ends_with("/")) {
    return path + filename;
  } else {
    return path + "/" + filename;
  }
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
  if (!existsFile(filePath)) {
    throw NitFsError(NitFsError::IdentifiableId::FILE_NOT_EXISTED,
                     "No such file: " + filePath);
  }
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

#endif
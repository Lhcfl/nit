#include "./utils.hpp"
#include "apis/apis.h"
#include <string>

#pragma once

#include <filesystem>
namespace NitFs {

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

} // namespace NitFs
#include "apis/apis.h"
#include "lib/nit_common.hpp"
#include "services/nit_checker.hpp"
#include <filesystem>
#include <string>
#include <vector>

#ifndef __H_NIT_STAGING_SERVICE
#define __H_NIT_STAGING_SERVICE

/**
 * we have only one staging area, so we can just use a service to describe it
 */
namespace NitStagingService {

inline NitLogger logger = NitLogger("staging");
constexpr std::string STAGE_PATH = ".nit/staged/";
const std::string STAGE_PATH_ABSOLUTE = NitFs::dirAbsolute(".nit/staged/");

inline void cleanAllStaged() {
  std::filesystem::remove_all(STAGE_PATH);
  std::filesystem::create_directories(STAGE_PATH);
}

inline std::vector<std::string> listAll() {
  NitCheckerService::ensureHasNitRepo();
  return NitFs::listFiles(STAGE_PATH_ABSOLUTE);
}

inline bool hasStaged(const std::string &filename) {
  return NitFs::existsFile(NitFs::fileIn(STAGE_PATH_ABSOLUTE, filename));
}

inline void removeOne(const std::string &filePath) {
  NitCheckerService::ensureHasNitRepo();

  if (hasStaged(filePath)) {
    std::filesystem::remove(NitFs::fileIn(STAGE_PATH_ABSOLUTE, filePath));
    std::filesystem::remove(filePath);
    return;
  } else {
    throw NitError("File not exist: " + filePath);
  }
}

inline void stageOne(const std::string &filePath) {
  logger.disable();
  logger.debug("Staging", filePath, "to", UsefulApi::cwd());
  NitCheckerService::ensureHasNitRepo();

  if (!NitCheckerService::hasFile(filePath)) {
    if (hasStaged(filePath)) {
      std::filesystem::remove(NitFs::fileIn(STAGE_PATH_ABSOLUTE, filePath));
      return;
    } else {
      throw NitError("File not exist: " + filePath);
    }
  }

  if (std::filesystem::is_directory(filePath)) {
    throw NitNotImplementedError(
        NitNotImplementedError::NotImplementId::ADD_A_DIRECTORY,
        "Staging a dir is not Implemented");
  }

  NitFs::writeToFile(NitFs::fileIn(STAGE_PATH_ABSOLUTE, filePath),
                     NitFs::readFromFile(filePath));
}

inline void stageAll(const std::string &directoryPath) {
  logger.debug("Staging", directoryPath);
  NitCheckerService::ensureHasNitRepo();

  cleanAllStaged();

  for (const auto &filename : NitFs::listFiles(directoryPath)) {
    NitFs::writeToFile(NitFs::fileIn(STAGE_PATH_ABSOLUTE, filename),
                       NitFs::readFromFile(filename));
  }
}

inline void rebuildStaged() {
  cleanAllStaged();
  stageAll(NitFs::cwd());
}
} // namespace NitStagingService

#endif
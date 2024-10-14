#include "apis/apis.h"
#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include "services/nit_checker.hpp"
#include <filesystem>
#include <ranges>
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

inline std::vector<std::string> listAll() {
  NitCheckerService::ensureHasNitRepo();
  return NitFs::listFiles(STAGE_PATH_ABSOLUTE);
}

inline void stageOne(const std::string &filePath) {
  logger.disable();
  logger.debug("Staging", filePath, "to", UsefulApi::cwd());
  NitCheckerService::ensureHasNitRepo();
  NitCheckerService::ensureHasFile(filePath);

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

  for (const auto &filename : NitFs::listFiles(directoryPath)) {
    NitFs::writeToFile(NitFs::fileIn(STAGE_PATH_ABSOLUTE, filename),
                       NitFs::readFromFile(filename));
  }
}

} // namespace NitStagingService

#endif
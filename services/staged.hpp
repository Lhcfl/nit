#include "apis/apis.h"
#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include "services/nit_checker.hpp"
#include <string>

#ifndef __H_NIT_STAGING_SERVICE
#define __H_NIT_STAGING_SERVICE

/**
 * we have only one staging area, so we can just use a service to describe it
 */
namespace NitStagingService {

const inline NitLogger logger = NitLogger("staging");
constexpr std::string STAGE_PATH = ".nit/staged/";

inline void stageOne(const std::string &filePath) {
  logger.debug("Staging", filePath, "to", UsefulApi::cwd());
  NitCheckerService::ensureHasNitRepo();
  NitCheckerService::ensureHasFile(filePath);

  NitFs::writeToFile(NitFs::dirRelative(STAGE_PATH) + filePath,
                     NitFs::readFromFile(filePath));
}

inline void stageAll(const std::string &directoryPath) {
  logger.debug("Saving data to", directoryPath);
  NitCheckerService::ensureHasNitRepo();

  auto shouldStageFiles = UsefulApi::listFilesInDirectory(directoryPath);
  for (const auto &filename : shouldStageFiles) {
    NitFs::writeToFile(NitFs::dirRelative(STAGE_PATH) + filename,
                       NitFs::readFromFile(filename));
  }
}

} // namespace NitStagingService

#endif
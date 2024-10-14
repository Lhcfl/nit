#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include "models/status.hpp"
#include "services/nit_checker.hpp"
#include "services/staged.hpp"
#include <string>
#include <vector>

#ifndef __H_NIT_STATUS_SERVICE
#define __H_NIT_STATUS_SERVICE

namespace NitStatusService {

using FileStatus = NitStatusModel::FileStatus;
using FileStatusNamePair = NitStatusModel::FileStatusNamePair;

/**
 * @param lazy Don't read file, only calculate the files is existed or not
 */
inline FileStatus compareFile(const std::string &sourceFile,
                              const std::string &compareWithFile,
                              FileStatus newFileIs = FileStatus::NEWFILE,
                              bool lazy = false) {
  bool sourceExists = NitFs::existsFile(sourceFile);
  bool destExists = NitFs::existsFile(compareWithFile);
  if (sourceExists && !destExists) {
    return newFileIs;
  }
  if (!sourceExists && destExists) {
    return FileStatus::REMOVED;
  }
  if (!sourceExists && !destExists) {
    throw NitError(
        "Unexpected compare: sourcefile and destfile neither exists: " +
        sourceFile + ", " + compareWithFile);
  }

  if (lazy) {
    // We don't need read the file again
    return FileStatus::UNCHANGED;
  } else if (NitFs::readFromFile(sourceFile) ==
             NitFs::readFromFile(compareWithFile)) {
    return FileStatus::UNCHANGED;
  } else {
    return FileStatus::MODIFIED;
  }
}

inline NitStatusModel getStatus() {
  NitCheckerService::ensureHasNitRepo();

  NitStatusModel res;

  for (auto &filename : NitFs::listFiles()) {
    auto status = compareFile(
        NitFs::fileIn(NitFs::cwd(), filename),
        NitFs::fileIn(NitStagingService::STAGE_PATH_ABSOLUTE, filename),
        FileStatus::UNTRACKED);
    res.workingDirStatus.emplace_back(
        std::move(FileStatusNamePair(filename, status)));
  }

  for (auto &filename : NitStagingService::listAll()) {
    auto status = compareFile(
        NitFs::fileIn(NitStagingService::STAGE_PATH_ABSOLUTE, filename),
        NitFs::fileIn(NitFs::cwd(), filename), FileStatus::REMOVED, true);

    if (status == FileStatus::REMOVED)
      res.workingDirStatus.emplace_back(
          std::move(FileStatusNamePair(filename, status)));

    // TODO: handle commit
  }

  return res;
}

} // namespace NitStatusService

#endif
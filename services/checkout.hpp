#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include "services/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/staged.hpp"
#include "services/status.hpp"
#include <filesystem>
#include <map>
#include <string>

#ifndef __H_NIT_CHECKOUT_SERVICE
#define __H_NIT_CHECKOUT_SERVICE

namespace NitCheckoutService {

inline const NitCommitModel checkout(const NitCommitModel &to,
                                     bool hard = false) {
  NitCheckerService::ensureHasNitRepo();

  auto repo = NitRepoService::getRepo();
  const NitCommitModel head = NitRepoService::headCommit();
  auto status = NitStatusService::getStatus();
  status.calculateStatus();

  if (hard) {
    for (auto f : NitFs::listFiles()) {
      std::filesystem::remove(f);
    }
    NitStagingService::rebuildStaged();
  } else {
    // Handle Untracked/Added
    for (auto file : status.workingDirStatus) {
      if (file.status == NitStatusModel::FileStatus::UNTRACKED &&
          to.files.contains(file.filename)) {
        throw NitCommand::ExecError(
            "untracked " + file.filename +
            " will be covered! please remove it or add and commit it");
      }
    }
    for (auto file : status.stagingAreaStatus) {
      if (file.status == NitStatusModel::FileStatus::NEWFILE &&
          to.files.contains(file.filename)) {
        throw NitCommand::ExecError(
            "added " + file.filename +
            " will be covered! please remove it or add and commit it");
      }
    }

    std::map<std::string, bool> shouldNotAdd;
    auto checkUnchanged = [&](const std::string &filename) {
      if (status.stagingAreaStatMap[filename] !=
              NitStatusModel::FileStatus::UNCHANGED ||
          status.workingDirStatMap[filename] !=
              NitStatusModel::FileStatus::UNCHANGED) {
        throw NitCommand::ExecError(filename +
                                    " is modified and will be covered! "
                                    "please restore it or add and commit it");
      }
    };

    for (auto file : head.files) {
      const auto filename = file.first;
      const auto blob = NitBlobModel::loadFrom(file.second);
      // both have
      if (to.files.contains(filename)) {
        const auto blobTo = NitBlobModel::loadFrom(to.files.at(filename));

        if (blobTo.data == blob.data) {
          // Two files are same, okay to checkout
          shouldNotAdd[filename] = true;
        } else {
          checkUnchanged(filename);
        }
      } else {
        checkUnchanged(filename);
      }

      std::filesystem::remove(NitFs::fileIn(NitFs::cwd(), filename));
    }

    for (auto file : to.files) {
      const auto filename = file.first;
      const auto blob = NitBlobModel::loadFrom(file.second);

      NitFs::writeToFile(filename, blob.data);

      if (!shouldNotAdd[filename]) {
        NitStagingService::stageOne(filename);
      }
    }
  }

  repo.head = to.hash;
  repo.save();

  return to;
}

inline const NitCommitModel checkout(const std::string hash,
                                     bool hard = false) {
  NitCheckerService::ensureHasNitRepo();

  if (hash.size() == 40) {
    // full size
    return checkout(NitCommitModel::loadFrom(hash));
  } else if (hash.size() < 6) {
    throw NitCommand::ExecError("Commit hash provided is too short: " + hash);
  } else {
    for (auto h : NitCommitService::listAllHash()) {
      if (h.starts_with(hash)) {
        return checkout(NitCommitModel::loadFrom(h), hard);
      }
    }
    throw NitCommand::ExecError("No such commit: " + hash);
  }
}

} // namespace NitCheckoutService

#endif
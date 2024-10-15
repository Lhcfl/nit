#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include "services/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/staged.hpp"
#include "services/status.hpp"
#include <filesystem>
#include <string>

#ifndef __H_NIT_CHECKOUT_SERVICE
#define __H_NIT_CHECKOUT_SERVICE

namespace NitCheckoutService {

inline const NitCommitModel checkout(const NitCommitModel &to,
                                     bool hard = false) {
  NitCheckerService::ensureHasNitRepo();

  auto repo = NitRepoService::getRepo();
  auto status = NitStatusService::getStatus();

  if (hard) {
    for (auto f : NitFs::listFiles()) {
      std::filesystem::remove(f);
    }
    NitStagingService::rebuildStaged();
  } else {
    throw NitError("Not implemted"); // TODO
    // for (auto f : headCommit.files) {
    //   std::filesystem::remove(f);
    // }
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
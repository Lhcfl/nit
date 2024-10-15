#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include "services/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/staged.hpp"
#include "services/status.hpp"
#include <string>

#ifndef __H_NIT_CHECKOUT_SERVICE
#define __H_NIT_CHECKOUT_SERVICE

namespace NitCheckoutService {

inline NitCommitModel checkout(const NitCommitModel &to) {
  NitCheckerService::ensureHasNitRepo();

  auto status = NitStatusService::getStatus();
  return to;
}

inline NitCommitModel checkout(const std::string hash) {
  NitCheckerService::ensureHasNitRepo();

  if (hash.size() == 40) {
    // full size
    return checkout(NitCommitModel::loadFrom(hash));
  } else if (hash.size() < 6) {
    throw NitCommand::ExecError("Commit hash provided is too short: " + hash);
  } else {
    for (auto h : NitCommitService::listAllHash()) {
      if (h.starts_with(hash)) {
        return checkout(NitCommitModel::loadFrom(h));
      }
    }
    throw NitCommand::ExecError("No such commit: " + hash);
  }
}

} // namespace NitCheckoutService

#endif
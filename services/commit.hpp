#include "lib/nit_common.hpp"
#include "models/commit.hpp"

#ifndef __H_NIT_COMMIT_SERVICE
#define __H_NIT_COMMIT_SERVICE

namespace NitCommitService {

inline void logCommit(const NitCommitModel &c) {
  NitLogger logger("", "");
  logger.log("[", std::string_view(c.hash).substr(0, 8), "] ", c.message);
}

inline NitCommitModel commit() {
  NitCommitModel res;
  return res;
}

} // namespace NitCommitService

#endif
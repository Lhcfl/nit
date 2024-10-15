#include "lib/nit_common.hpp"
#include "models/commit.hpp"

#ifndef __H_NIT_COMMIT_SERVICE
#define __H_NIT_COMMIT_SERVICE

namespace NitCommitService {

inline void logOneCommit(const NitCommitModel &commit) {
  NitLogger logger("", "");
  logger.log("[", std::string_view(commit.hash).substr(0, 8), "] ",
             commit.message);
  logger.log(commit.createdAt);
}

inline void logOneCommitDetailed(const NitCommitModel &commit) {
  NitLogger logger;
  logger.log(NitColor::yellow("commit: " + commit.hash));
  logger.log("date:", commit.createdAt);
  logger.log("message:", commit.message);
}

inline void logManyCommit(const NitCommitModel &commit) {
  NitLogger logger("", "");

  auto hashNow = commit.hash;
  while (hashNow != NitCommitModel::NULL_COMMIT) {
    const auto c = NitCommitModel::loadFrom(hashNow);
    logOneCommitDetailed(c);
    hashNow = c.pre;
    logger.newLine();
  }
}

inline NitCommitModel commit() {
  NitCommitModel res;
  // We allow empty commit so...
  return res;
}

} // namespace NitCommitService

#endif
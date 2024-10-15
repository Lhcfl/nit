#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/nit_repo.hpp"
#include "services/staged.hpp"

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

inline NitCommitModel makeCommit(const std::string &message) {
  NitCheckerService::ensureHasNitRepo();
  auto repo = NitRepoService::getRepo();

  auto res = NitCommitModel::createAndSaveFrom(repo.head, message, []() {
    NitCommitModel::Tree files;
    auto stagingArea = NitStagingService::listAll();
    for (auto &stagedFileName : stagingArea) {
      auto blob = NitBlobModel::createAndSaveFrom(NitFs::fileIn(
          NitStagingService::STAGE_PATH_ABSOLUTE, stagedFileName));
      files[stagedFileName] = blob.hash;
    }
    return files;
  }());

  repo.head = res.hash;
  repo.save();
  return res;
}

} // namespace NitCommitService

#endif
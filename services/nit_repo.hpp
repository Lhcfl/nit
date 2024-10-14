#include "apis/apis.h"
#include "commands/base.hpp"
#include "lib/extern/json.hpp"
#include "lib/nit_common.hpp"
#include "models/blob.hpp"
#include "models/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/staged.hpp"
#include <filesystem>
#include <string>
#include <vector>

#ifndef __H_NIT_REPO_SERVICE
#define __H_NIT_REPO_SERVICE

/**
 * we have only one repo info, so we can just use a service to describe it
 */
namespace NitRepoService {

struct NitRepo {
  /** Head commit hash */
  std::string head;
};

inline void to_json(nlohmann::json &j, const NitRepo &p) {
  j = nlohmann::json{{"head", p.head}};
}

inline void from_json(const nlohmann::json &j, NitRepo &p) {
  j.at("head").get_to(p.head);
}

inline NitRepo getRepo() {
  auto j = nlohmann::json::parse(
      NitFs::readFromFile(NitFs::fileIn(".nit", "repo.json")));
  NitRepo res;
  res = j;
  return res;
}
inline void writeRepoData(const NitRepo &repo) {
  nlohmann::json j = repo;
  NitFs::writeToFile(NitFs::fileIn(".nit", "repo.json"), j.dump());
}

inline NitCommitModel initialCommit() {
  NitCommitModel commit("initial commit");
  commit.save();
  return commit;
}

inline NitRepo initialize() {
  if (NitCheckerService::hasNitRepo()) {
    throw NitCommand::ExecError(
        "A Nit version-control system already exists in the "
        "current directory.");
  }

  std::filesystem::create_directory(".nit");
  std::filesystem::create_directory(".nit/staged");
  std::filesystem::create_directory(".nit/commits");
  std::filesystem::create_directory(".nit/blobs");

  NitRepo repo;
  auto commit = initialCommit();
  repo.head = commit.hash;

  writeRepoData(repo);

  return repo;
}
} // namespace NitRepoService

#endif
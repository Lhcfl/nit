#include "apis/apis.h"
#include "lib/extern/json.hpp"
#include "lib/nit_common.hpp"
#include <map>
#include <string>

#ifndef __H_NIT_MODEL_COMMIT
#define __H_NIT_MODEL_COMMIT

class NitCommitModel {
  void deserializeFrom(const std::string &jsonStr) {
    auto parsed = nlohmann::json::parse(jsonStr);

    next = parsed["next"].template get<std::string>();
    next = parsed["pre"].template get<std::string>();
    next = parsed["message"].template get<std::string>();
    files = parsed["files"].template get<Tree>();
  };

  std::string serialize() {
    nlohmann::json j;
    j["next"] = next;
    j["pre"] = pre;
    j["message"] = message;
    j["files"] = files;

    return j.dump();
  }

public:
  static constexpr std::string COMMIT_PATH = ".nit/blobs";
  static const std::string COMMIT_PATH_ABSOLUTE;
  static const NitLogger logger;

  using Tree = std::map<std::string, std::string>;

  std::string hash;
  std::string next;
  std::string pre;
  std::string message;

  /** files[filename] = blobhash */
  Tree files;

  static NitCommitModel loadFrom(const std::string &hash) {
    std::string data =
        NitFs::readFromFile(NitFs::fileIn(COMMIT_PATH_ABSOLUTE, hash));
    auto checkHash = UsefulApi::hash(data);

    if (hash != checkHash) {
      throw NitError("Commit: Hash check failed!\n"
                     "  provided hash: " +
                     hash +
                     "\n"
                     "      data hash: " +
                     checkHash);
    }

    NitCommitModel res;
    res.deserializeFrom(data);

    return res;
  }

  void save() {
    logger.debug("Saving commit", hash, "to", UsefulApi::cwd());

    auto serialized = serialize();
    hash = UsefulApi::hash(serialized);

    NitFs::writeToFile(hash, serialized);
  }
};

const inline NitLogger NitCommitModel::logger = NitLogger("commit");
const inline std::string NitCommitModel::COMMIT_PATH_ABSOLUTE =
    NitFs::dirAbsolute(COMMIT_PATH);

#endif
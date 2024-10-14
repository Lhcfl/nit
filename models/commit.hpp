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
    pre = parsed["pre"].template get<std::string>();
    message = parsed["message"].template get<std::string>();
    files = parsed["files"].template get<Tree>();
  };

  std::string serialize() {
    // logger.debug("Serializing...");
    nlohmann::json j;
    j["next"] = next;
    j["pre"] = pre;
    j["message"] = message;
    j["files"] = files;

    return j.dump();
  }

public:
  static constexpr std::string COMMIT_PATH = ".nit/commits";
  static constexpr std::string NULL_COMMIT = "#null";
  static const std::string COMMIT_PATH_ABSOLUTE;
  static const NitLogger logger;

  using Tree = std::map<std::string, std::string>;

  std::string hash;
  std::string next;
  std::string pre;
  std::string message;

  /** files[filename] = blobhash */
  Tree files;

  NitCommitModel(const std::string &msg = "")
      : next(NULL_COMMIT), pre(NULL_COMMIT), message(msg) {}

  static NitCommitModel loadFrom(const std::string &hash) {
    // logger.debug("Loading commit", hash);

    std::string data =
        NitFs::readFromFile(NitFs::fileIn(COMMIT_PATH_ABSOLUTE, hash));
    auto checkHash = UsefulApi::hash(data);

    // logger.debug(data);

    if (hash != checkHash) {
      throw NitError("Commit: Hash check failed!\n"
                     "  provided hash: " +
                     hash +
                     "\n"
                     "      data hash: " +
                     checkHash);
    }

    NitCommitModel res;
    res.hash = hash;
    res.deserializeFrom(data);

    return res;
  }

  void save() {
    logger.debug("Saving commit", hash, "to", UsefulApi::cwd());

    auto serialized = serialize();
    hash = UsefulApi::hash(serialized);

    logger.debug("hash generated: ", hash);

    NitFs::writeToFile(NitFs::fileIn(COMMIT_PATH_ABSOLUTE, hash), serialized);
  }
};

const inline NitLogger NitCommitModel::logger = NitLogger("commit");
const inline std::string NitCommitModel::COMMIT_PATH_ABSOLUTE =
    NitFs::dirAbsolute(COMMIT_PATH);

#endif
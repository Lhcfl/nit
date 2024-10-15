#include "apis/apis.h"
#include "lib/extern/json.hpp"
#include "lib/nit_common.hpp"
#include <chrono>
#include <cstdint>
#include <map>
#include <stdexcept>
#include <string>

#ifndef __H_NIT_MODEL_COMMIT
#define __H_NIT_MODEL_COMMIT

class NitCommitModel {
  void deserializeFrom(const std::string &jsonStr) {
    auto parsed = nlohmann::json::parse(jsonStr);

    // next = parsed["next"].template get<std::string>();
    pre = parsed["pre"].template get<std::string>();
    message = parsed["message"].template get<std::string>();
    files = parsed["files"].template get<Tree>();

    uint64_t timestamp = parsed["createdAt"].template get<uint64_t>();
    createdAt = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::from_time_t(timestamp / 1000));
  };

  std::string serialize() {
    // logger.debug("Serializing...");
    nlohmann::json j;
    // j["next"] = next;
    j["pre"] = pre;
    j["message"] = message;
    j["files"] = files;
    j["createdAt"] = createdAt.time_since_epoch().count();

    return j.dump();
  }

  NitCommitModel &save() {
    logger.debug("Saving commit", hash, "to", UsefulApi::cwd());

    auto serialized = serialize();
    hash = UsefulApi::hash(serialized);

    logger.debug("hash generated: ", hash);

    NitFs::writeToFile(NitFs::fileIn(COMMIT_PATH_ABSOLUTE, hash), serialized);
    return *this;
  }

public:
  static constexpr std::string COMMIT_PATH = ".nit/commits";
  static constexpr std::string NULL_COMMIT = "#null";
  static constexpr int EMPTY_COMMIT = -1;
  static const std::string COMMIT_PATH_ABSOLUTE;
  static const NitLogger logger;

  using Tree = std::map<std::string, std::string>;

  using MsTime = std::chrono::time_point<std::chrono::system_clock,
                                         std::chrono::milliseconds>;

  std::string hash;
  // std::string next; // We can't record next
  std::string pre;
  std::string message;
  MsTime createdAt;

  /** files[filename] = blobhash */
  Tree files;

  NitCommitModel(const std::string &msg = "") : pre(NULL_COMMIT), message(msg) {
    createdAt = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());
  }
  NitCommitModel(const std::string &msg, const std::string &pre, Tree &&files)
      : pre(pre), message(msg), files(files) {
    createdAt = std::chrono::time_point_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now());
  }

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

  static NitCommitModel createAndSaveFrom(const std::string &pre,
                                          const std::string &message,
                                          Tree &&files) {
    return NitCommitModel(message, pre, std::move(files)).save();
  };
  static NitCommitModel createAndSaveFrom(const std::string &pre_is_null,
                                          const std::string &message,
                                          int EMPTY) {
    if (EMPTY != EMPTY_COMMIT || pre_is_null != NULL_COMMIT) {
      throw std::invalid_argument(
          "You can only provide a empty commit for this constructor");
    }
    return NitCommitModel(message).save();
  };
};

const inline NitLogger NitCommitModel::logger = NitLogger("commit");
const inline std::string NitCommitModel::COMMIT_PATH_ABSOLUTE =
    NitFs::dirAbsolute(COMMIT_PATH);

#endif
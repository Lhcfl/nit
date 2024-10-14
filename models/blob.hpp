#include "apis/apis.h"
#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include <string>

#ifndef __H_NIT_MODEL_BLOB
#define __H_NIT_MODEL_BLOB

class NitBlobModel {
public:
  static constexpr std::string BLOB_PATH = ".nit/blobs";
  static const std::string BLOB_PATH_ABSOLUTE;
  static const NitLogger logger;
  std::string hash;
  std::string data;

  NitBlobModel(const std::string &data) : data(data) {
    hash = UsefulApi::hash(data);
  }
  NitBlobModel(const std::string &data, const std::string &hash)
      : data(data), hash(hash) {
    auto checkHash = UsefulApi::hash(data);
    if (hash != checkHash) {
      throw NitError("Hash check failed!\n"
                     "  provided hash: " +
                     hash +
                     "\n"
                     "      data hash: " +
                     checkHash);
    }
  }

  static NitBlobModel loadFrom(const std::string &hash) {
    return NitBlobModel(
        NitFs::readFromFile(NitFs::fileIn(BLOB_PATH_ABSOLUTE, hash)), hash);
  }

  void save() {
    logger.debug("Saving blob", hash, " data to", UsefulApi::cwd());
    NitFs::writeToFile(NitFs::fileIn(BLOB_PATH_ABSOLUTE, hash), data);
  }
};

const inline NitLogger NitBlobModel::logger = NitLogger("blob");
const inline std::string NitBlobModel::BLOB_PATH_ABSOLUTE =
    NitFs::dirAbsolute(BLOB_PATH);

#endif
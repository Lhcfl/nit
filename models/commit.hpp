#include "apis/apis.h"
#include "lib/nit_common.hpp"
#include <string>

#ifndef __H_NIT_MODEL_COMMIT
#define __H_NIT_MODEL_COMMIT

class NitCommitModel {
public:
  static const NitLogger logger;

  std::string hash;
  std::string message;

  void save() { logger.debug("Saving data to", UsefulApi::cwd()); }
};

const inline NitLogger NitCommitModel::logger = NitLogger("commit");

#endif
#include "apis/apis.h"
#include "lib/nit_common.hpp"
#include <string>

#ifndef NIT_MODEL_STAGED
#define NIT_MODEL_STAGED

class NitStagedModel {
public:
  static const NitLogger logger;

  void save() { logger.debug("Saving data to", UsefulApi::cwd()); }
};

const inline NitLogger NitStagedModel::logger = NitLogger("staging");

#endif
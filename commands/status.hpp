#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "services/status.hpp"
#include <string>

#ifndef __H_NIT_COMMAND_STATUS
#define __H_NIT_COMMAND_STATUS

namespace NitCommand {
class Status : public Base {
public:
  Status() : Base() {}

  void exec() const override {
    NitLogger logger;
    auto status = NitStatusService::getStatus();
    auto allStaged = NitStatusService::actuallyStagedFiles(status);

    if (allStaged.size() > 0) {
      logger.log("======= NIT STAGING AREA ========");
      for (auto &item : allStaged) {
        logger.log(item);
      }
      logger.newLine();
    }

    logger.log(Utils::rtrim(status.calculateStatus()), NitLogger::NO_ENDL);
  }
};

} // namespace NitCommand

#endif
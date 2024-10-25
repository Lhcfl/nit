#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "services/staged.hpp"
#include <array>
#include <string>

#ifndef __H_NIT_COMMAND_RM
#define __H_NIT_COMMAND_RM

namespace NitCommand {
class Rm : public Base {
public:
  Rm(CommandArgs &&args) : Base(std::move(args)) {}

  static constexpr std::array<std::string, 1> ALIASES{"rm"};

  void exec() const override {
    NitLogger logger("nit rm");

    if (args.size() == 0) {
      throw ExecError("You must at least remove one file");
    }

    logger.debug("args is", args);

    for (const auto &fileName : args) {
      try {
        NitStagingService::removeOne(fileName);
      } catch (NitNotImplementedError e) {
        if (e.getId() ==
            NitNotImplementedError::NotImplementId::ADD_A_DIRECTORY) {
          logger.warn("Adding a directory is not supported. Ingoring",
                      fileName);
        } else {
          throw e;
        }
      }
    }
  }
};

} // namespace NitCommand

#endif
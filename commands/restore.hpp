#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "services/staged.hpp"
#include <array>
#include <string>

#ifndef __H_NIT_COMMAND_RESTORE
#define __H_NIT_COMMAND_RESTORE

namespace NitCommand {
class Restore : public Base {
public:
  Restore(CommandArgs &&args) : Base(std::move(args)) {}

  static constexpr std::array<std::string, 1> ALIASES{"restore"};

  void exec() const override {
    NitLogger logger("nit restore");

    if (args.size() == 0) {
      throw ExecError("You must at least restore one file");
    }

    logger.debug("args is", args);

    // if (args | Utils::Some<std::string>([](const std::string &fn) {
    //       return fn == "*" || fn == ".";
    //     })) {
    //   return NitStagingService::stageAll(".");
    // }

    for (const auto &fileName : args) {
      try {
        NitStagingService::stageOne(fileName);
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
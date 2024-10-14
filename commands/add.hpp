#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include "services/staged.hpp"
#include <string>

#ifndef __H_NIT_COMMAND_ADD
#define __H_NIT_COMMAND_ADD

namespace NitCommand {
class Add : public Base {
public:
  Add(CommandArgs &&args) : Base(std::move(args)) {}

  void exec() const {
    if (args.size() == 0) {
      throw ExecError("You must at least add one file");
    }

    if (args | Utils::Some<std::string>([](const std::string &fn) {
          return fn == "*" || fn == ".";
        })) {
      return NitStagingService::stageAll(".");
    }

    for (const auto &fileName : args) {
      NitStagingService::stageOne(fileName);
    }
  }
};

} // namespace NitCommand

#endif
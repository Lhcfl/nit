#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <string>

#ifndef NIT_COMMAND_COMMIT
#define NIT_COMMAND_COMMIT

namespace NitCommand {
class Commit : public Base {
public:
  Commit(CommandArgs &&args) : Base(std::move(args)) {}

  void exec() const {
    if (args.size() == 0) {
      throw ExecError("You must provide a commit message");
    }

    throw ExecError("Not implemented: commit with message \"" + args[0] + "\"");
  }
};

} // namespace NitCommand

#endif
#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "services/commit.hpp"
#include <string>

#ifndef __H_NIT_COMMAND_COMMIT
#define __H_NIT_COMMAND_COMMIT

namespace NitCommand {
class Commit : public Base {
public:
  Commit(CommandArgs &&args) : Base(std::move(args)) {}

  void exec() const {
    if (args.size() == 0) {
      throw ExecError("You must provide a commit message");
    }

    auto commit = NitCommitService::makeCommit(args[0]);
    NitCommitService::logOneCommit(commit);
  }
};

} // namespace NitCommand

#endif
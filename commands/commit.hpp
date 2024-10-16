#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "services/commit.hpp"
#include <array>
#include <string>

#ifndef __H_NIT_COMMAND_COMMIT
#define __H_NIT_COMMAND_COMMIT

namespace NitCommand {
class Commit : public Base {
public:
  Commit(CommandArgs &&args) : Base(std::move(args)) {}

  static constexpr std::array<std::string, 1> ALIASES{"commit"};

  void exec() const override {
    if (args.size() == 0) {
      throw ExecError("Please enter a commit message.");
    }
    bool allow_empty = false;

    for (const auto &arg : args) {
      if (arg == "--allow-empty") {
        allow_empty = true;
      }
    }

    auto commit = NitCommitService::makeCommit(args[0], allow_empty);
    NitCommitService::logOneCommit(commit);
  }
};

} // namespace NitCommand

#endif
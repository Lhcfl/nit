#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "services/checkout.hpp"
#include "services/commit.hpp"
#include <string>

#ifndef __H_NIT_COMMAND_CHECKOUT
#define __H_NIT_COMMAND_CHECKOUT

namespace NitCommand {
class Checkout : public Base {
public:
  Checkout(CommandArgs &&args) : Base(std::move(args)) {}

  void exec() const {
    if (args.size() == 0) {
      throw ExecError("You should enter a commit hash.");
    }

    bool hard = false;
    for (const auto &arg : args) {
      if (arg == "--hard") {
        hard = true;
      }
    }

    auto commit = NitCheckoutService::checkout(args[0], hard);
    NitCommitService::logOneCommit(commit);
  }
};

} // namespace NitCommand

#endif
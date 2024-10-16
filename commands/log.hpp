#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include "services/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/nit_repo.hpp"
#include <array>
#include <string>

#ifndef __H_NIT_COMMAND_LOG
#define __H_NIT_COMMAND_LOG

namespace NitCommand {
class Log : public Base {
public:
  Log(CommandArgs &&_) : Base() {}

  static constexpr std::array<std::string, 1> ALIASES{"log"};

  void exec() const override {
    NitCheckerService::ensureHasNitRepo();

    NitLogger logger("Nit");
    auto repo = NitRepoService::getRepo();

    NitCommitService::logManyCommit(NitCommitModel::loadFrom(repo.head));
  }
};

} // namespace NitCommand

#endif
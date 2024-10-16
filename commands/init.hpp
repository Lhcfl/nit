#include "apis/apis.h"
#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include "services/commit.hpp"
#include "services/nit_checker.hpp"
#include "services/nit_repo.hpp"
#include <array>
#include <string>

#ifndef __H_NIT_COMMAND_INIT
#define __H_NIT_COMMAND_INIT

namespace NitCommand {
class Init : public Base {
public:
  Init(CommandArgs &&_) : Base() {}

  static constexpr std::array<std::string, 1> ALIASES{"init"};

  void exec() const override {
    NitLogger logger("Nit");
    logger.info("Initializing a new nit repo at", UsefulApi::cwd());
    auto repo = NitRepoService::initialize();
    logger.success("Successfully Initialized .nit in", UsefulApi::cwd());
    logger.info("initial commit created");
    logger.newLine();
    NitCommitService::logOneCommit(NitCommitModel::loadFrom(repo.head));
  }
};

} // namespace NitCommand

#endif
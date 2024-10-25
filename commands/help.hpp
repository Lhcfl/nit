#include "commands/base.hpp"
#include "config/config.h"
#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <array>
#include <string>

#ifndef __H_NIT_COMMAND_HELP
#define __H_NIT_COMMAND_HELP

namespace NitCommand {
class Help : public Base {
public:
  Help(CommandArgs &&_) : Base() {}

  static constexpr std::array<std::string, 2> ALIASES{"help", "h"};

  void exec() const override {
    NitLogger logger;
    logger.log("========= Nit Help ==========");
    logger.log("Nit version", NitProjectConfig::VERSION);
    logger.newLine();
    logger.log("Commands:");
    logger.log("  init");
    logger.log("  commit [msg]");
    logger.log("  add [file1] [other files...?]");
    logger.log("  rm [file1] [other files...?]");
    logger.log("  status");
    logger.log("  checkout [hash]");
    logger.log("  log");
    logger.log("  help");
  }
};

} // namespace NitCommand

#endif
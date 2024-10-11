#include "commands/base.hpp"
#include "commands/commit.hpp"
#include "commands/help.hpp"

#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <memory>
#include <string>
#include <vector>

#ifndef NIT_COMMAND_PARSER_SERVICE
#define NIT_COMMAND_PARSER_SERVICE

namespace NitCommandParserService {

typedef std::unique_ptr<NitCommand::Base> Command;

inline Command createFrom(int argc, char **argv) {
  NitLogger logger("Command");

  if (argc == 1) {
    return Command(new NitCommand::Help());
  }

  std::string commandName = argv[1];
  std::vector<std::string> commandArgs;
  for (int i = 2; i != argc; i++) {
    commandArgs.push_back(argv[i]);
  };

  if (commandName == "commit") {
    return Command(new NitCommand::Commit(std::move(commandArgs)));
  } else {
    throw NitCommand::ExecError("Cannot identify command `" + commandName +
                                "`");
  }
};

} // namespace NitCommandParserService

#endif
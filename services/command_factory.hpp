#include "commands/add.hpp"
#include "commands/base.hpp"
#include "commands/checkout.hpp"
#include "commands/commit.hpp"
#include "commands/help.hpp"
#include "commands/init.hpp"
#include "commands/log.hpp"
#include "commands/rm.hpp"
#include "commands/status.hpp"

#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef __H_NIT_COMMAND_FACTORY_SERVICE
#define __H_NIT_COMMAND_FACTORY_SERVICE

namespace NitCommandFactoryService {

#define __REGISTER_COMMAND(NAME)                                               \
  registerCommand(NitCommand::NAME::ALIASES, [&]() {                           \
    return std::make_unique<NitCommand::NAME>(std::move(commandArgs));         \
  })

typedef std::unique_ptr<NitCommand::Base> Command;
typedef std::function<Command()> CommandMaker;

static std::unordered_map<std::string, CommandMaker> CommandRegistry;

template <typename Alias>
inline void registerCommand(const Alias &command_names, CommandMaker &&maker) {
  for (auto &name : command_names) {
    CommandRegistry.insert_or_assign(name, maker);
  }
}

inline Command createFrom(int argc, char **argv) {
  NitLogger logger("Command");

  std::string commandName = argc == 1 ? "help" : argv[1];
  std::vector<std::string> commandArgs;
  for (int i = 2; i != argc; i++) {
    commandArgs.push_back(argv[i]);
  };

  __REGISTER_COMMAND(Help);
  __REGISTER_COMMAND(Init);
  __REGISTER_COMMAND(Log);
  __REGISTER_COMMAND(Status);
  __REGISTER_COMMAND(Commit);
  __REGISTER_COMMAND(Checkout);
  __REGISTER_COMMAND(Add);
  __REGISTER_COMMAND(Rm);

  if (CommandRegistry.count(commandName) > 0) {
    return CommandRegistry[commandName]();
  } else {
    throw NitCommand::ExecError("Cannot identify command `" + commandName +
                                "`");
  }
};

} // namespace NitCommandFactoryService

#endif
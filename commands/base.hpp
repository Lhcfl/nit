#include "lib/nit_common.hpp"
#include "models/commit.hpp"
#include <string>
#include <vector>

#ifndef __H_NIT_COMMAND_BASE
#define __H_NIT_COMMAND_BASE

namespace NitCommand {
using CommandArgs = std::vector<std::string>;

class ExecError : public NitError {
public:
  ExecError(const std::string &message) : NitError(message) {}
};

class Base {
protected:
  CommandArgs args;

public:
  Base() : args(){};
  Base(CommandArgs &&args) : args(args){};
  virtual void exec() const = 0;
};
} // namespace NitCommand

#endif
#include "apis/apis.h"
#include "commands/base.hpp"
#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include "models/commit.hpp"
#include <filesystem>
#include <string>

#ifndef NIT_COMMAND_INIT
#define NIT_COMMAND_INIT

namespace NitCommand {
class Init : public Base {
public:
  Init() : Base() {}

  void exec() const {
    NitLogger logger("Nit");

    logger.debug("Initializing a new nit repo at", UsefulApi::cwd());
    logger.debug("Files in cwd", NitFs::readDir());

    if (NitFs::hasFile(".nit")) {
      throw ExecError("A Nit version-control system already exists in the "
                      "current directory.");
    }

    std::filesystem::create_directory(".nit");
    std::filesystem::create_directory(".nit/staged");
    std::filesystem::create_directory(".nit/commits");

    logger.success("Successfully Initialized .nit in", UsefulApi::cwd());
  }
};

} // namespace NitCommand

#endif
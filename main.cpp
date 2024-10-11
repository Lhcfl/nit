#include "lib/nit_common.hpp"
#include "services/command_parser.hpp"

int main(int argc, char **argv) {
  NitLogger logger;
  try {
    NitCommandParserService::createFrom(argc, argv)->exec();
  } catch (NitCommand::ExecError e) {
    logger.error(e.what());
    return 1;
  }
  return 0;
}
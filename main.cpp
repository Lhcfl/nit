#include "lib/nit_common.hpp"
#include "services/command_factory.hpp"
#include <exception>

int main(int argc, char **argv) {
  NitLogger logger;

  try {
    NitCommandFactoryService::createFrom(argc, argv)->exec();
  } catch (NitCommand::ExecError e) {
    logger.error(e.what());
    return 1;
  } catch (NitError e) {
    logger.error(e.what());
    return 1;
  } catch (std::exception e) {
    logger.error("A unrecoverable error happened:", "\n--------------\n",
                 e.what());
    return 1;
  }
  return 0;
}
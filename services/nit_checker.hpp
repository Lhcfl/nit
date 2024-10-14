#include "lib/nit_common.hpp"
#include "lib/nit_fs.hpp"
#include <string>

#ifndef __H_NIT_CHECKER_SERVICE
#define __H_NIT_CHECKER_SERVICE

#define _def_ensure_with(function, nit_message)                                \
  inline void ensureHas##function() {                                          \
    if (!(has##function)()) {                                                  \
      throw NitError(nit_message);                                             \
    }                                                                          \
  }
#define _def_ensure(function)                                                  \
  inline void ensureHas##function() {                                          \
    if (!(has##function)()) {                                                  \
      throw NitError("Check failed in has" #function " validation.");          \
    }                                                                          \
  }

namespace NitCheckerService {

using NitFs::hasFile;

inline bool hasNitRepo() { return NitFs::hasFile(".nit"); }
_def_ensure_with(NitRepo, "fatal: not a nit repository");

inline void ensureHasFile(const std::string &filename) {
  if (!hasFile(filename)) {
    throw NitError("File does not exist: " + filename);
  }
}

} // namespace NitCheckerService

#endif
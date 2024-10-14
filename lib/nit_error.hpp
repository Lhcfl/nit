#include <stdexcept>
#include <string>

#ifndef NIT_ERROR_HPP
#define NIT_ERROR_HPP

class NitError : public std::runtime_error {
public:
  NitError(const std::string &message = "UnknownError")
      : std::runtime_error(message) {}
  ~NitError() throw() {}
};

#endif
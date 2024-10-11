#include <exception>
#include <string>

#ifndef NIT_ERROR_HPP
#define NIT_ERROR_HPP

class NitError : std::exception {
  std::string message;

public:
  NitError(const std::string &message = "UnknownError") : message(message) {}
  ~NitError() throw() {}

  virtual const char *what() const throw() { return message.c_str(); }
};
#endif
#include <stdexcept>
#include <string>

#ifndef __H_NIT_ERROR_HPP
#define __H_NIT_ERROR_HPP

class NitError : public std::runtime_error {
public:
  NitError(const std::string &message = "UnknownError")
      : std::runtime_error(message) {}
  ~NitError() throw() {}
};

class NitNotImplementedError : public std::runtime_error {
public:
  enum class NotImplementId {
    ADD_A_DIRECTORY = 1,
  };

private:
  NotImplementId id;

public:
  NitNotImplementedError(NotImplementId id,
                         const std::string &message = "UnknownError")
      : std::runtime_error(message), id(id) {}
  ~NitNotImplementedError() throw() {}

  NotImplementId getId() { return id; }
};

#endif
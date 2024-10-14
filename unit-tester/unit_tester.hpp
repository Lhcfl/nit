#include <functional>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#ifndef LINCA_UNITTEST_H
#define LINCA_UNITTEST_H

namespace UnitTest {

template <class T> class Test {
private:
  bool success;
  std::stringstream output;
  std::stringstream expects;

public:
  T obj;
  Test(const std::string &desc) {
    success = true;
    output << "====================\n";
    output << "Test `" << desc << "`" << std::endl;
    output << "====================\n";
  }

#define want_that(val1, relation, val2)                                        \
  expect(__LINE__, #val1, #relation, #val2, val1, val2, (val1)relation(val2))

  template <typename VAL_TYPE1, typename VAL_TYPE2>
  Test &expect(int line, const std::string &val1_s, const std::string &relation,
               const std::string &val2_s, VAL_TYPE1 val1_v, VAL_TYPE2 val2_v,
               bool res) {
    if (!res) {
      expects << "  line: " << line << ", Assert `" << val1_s << " " << relation
              << " " << val2_s << "` failed:\n";
      expects << "    `" << val1_s << "`: " << val1_v << ",\n";
      expects << "    `" << val2_s << "`: " << val2_v << ";\n";
    }
    return *this;
  }

  template <typename VAL_TYPE>
  Test &expect_in(VAL_TYPE val1, std::vector<VAL_TYPE> val2) {
    for (auto &elem : val2) {
      if (val1 == elem) {
        return *this;
      }
    }
    expects << "  Assert `" << val1 << " in [";
    for (auto &elem : val2) {
      expects << elem;
    }
    expects << "]` failed;\n";
    return *this;
  }
  /**
   * Add a context to testcase
   */
  Test &context(const std::string &desc, std::function<void(Test &)> fn) {
    output << "\n\nContext `" << desc << "`:\n";
    fn(*this);
    return *this;
  }
  /**
   * Add a context to testcase
   */
  Test &context(const std::string &desc, const T &init,
                std::function<void(Test &)> fn) {
    output << "\n\nContext `" << desc << "`:\n";
    obj = init;
    fn(*this);
    return *this;
  }
  /**
   * Add a context to testcase
   */
  Test &context(const std::string &desc, const T &init) {
    output << "\n\nContext `" << desc << "`:\n";
    obj = init;
    return *this;
  }
  /**
   * Add a it test
   */
  Test &it(const std::string &desc, std::function<void(Test &)> fn) {
    try {
      fn(*this);
      if (expects.str() == "") {
        output << "\033[0;32m[OK]\033[0m it \"" << desc << "\";\n";
      } else {
        success = false;
        output << "\033[0;31m[FAILED]\033[0m it \"" << desc << "\":\n";
        output << expects.str();
        expects = std::stringstream();
      }
    } catch (const std::exception &err) {
      success = false;
      output << "\033[0;31m[ERROR]\033[0m it \"" << desc << "\":\n";
      output << "    Error: " << err.what();
      expects = std::stringstream();
      output << std::endl;
    }
    return *this;
  }

  std::string result() const { return output.str(); }
  bool succeed() const { return success; }

  ~Test() { std::cout << this->result(); }
};

} // namespace UnitTest

#endif
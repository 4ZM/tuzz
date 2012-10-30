#include "catch.hpp"

#include "tuzz/logger.hpp"

#include <sstream>


TEST_CASE( "tuzz/logger/ctor", "Construction" ) {
  std::stringstream ss;
  REQUIRE_NOTHROW(tuzz::logger l(ss));
}

TEST_CASE( "tuzz/logger/singleton", "Test the singleton interface" ) {
  std::stringstream ss;

  tuzz::logger& l1 = tuzz::logger::get_logger();
  tuzz::logger& l2 = tuzz::logger::get_logger();

  CHECK(&l1 == &l2);
}

TEST_CASE( "tuzz/logger/changetarget", "Test changing the output target" ) {
  std::stringstream ss1;

  tuzz::logger l(ss1);
  CHECK(ss1.str().empty());

  l.log("test1");

  std::string after_first_log = ss1.str();
  CHECK(!after_first_log.empty());

  std::stringstream ss2;
  l.reset_output_target(ss2);

  CHECK(ss2.str().empty());

  l.log("test2");

  CHECK(!ss2.str().empty());

  // Old stream is not influenced from second logging
  CHECK(ss1.str() == after_first_log);

  std::stringstream ss;
}

TEST_CASE( "tuzz/logger/stream", "Test stream logging" ) {
  std::stringstream ss;

  tuzz::logger l(ss);
  l << 1337 << 42.5 << std::string("str") << "c s t r" << tuzz::lend;

  std::string log = ss.str();
  CHECK(log.find("1337") != std::string::npos);
  CHECK(log.find("42.5") != std::string::npos);
  CHECK(log.find("str") != std::string::npos);
  CHECK(log.find("c s t r") != std::string::npos);
  CHECK(log.find("not in log") == std::string::npos);

}


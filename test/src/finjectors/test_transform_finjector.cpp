#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/finjectors/transform_finjector.hpp"

#include <memory>
#include <ctype.h>

TEST_CASE( "tuzz/finjectors/transform_finjector/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::transform_finjector finj([] (char x) { return x; } ));
}

TEST_CASE( "tuzz/finjectors/transform_finjectors/vanilla", "Testing with identiry transform" ) {

  const std::string text("fubar");

  auto fijn = std::make_shared<tuzz::transform_finjector>([] (char x) { return x; });
  std::string out = fijn->inject(text);

  CHECK(out == text);
}

TEST_CASE( "tuzz/finjectors/transform_finjectors/external_f", "Testing using the toupper function" ) {

  const std::string text("fUbAr");

  auto fijn = std::make_shared<tuzz::transform_finjector>(toupper);
  std::string out = fijn->inject(text);

  CHECK(out == "FUBAR");
}

TEST_CASE( "tuzz/finjectors/transform_finjectors/empty_input", "Runing on empty input" ) {

  const std::string text("");

  auto fijn = std::make_shared<tuzz::transform_finjector>(toupper);
  std::string out = fijn->inject(text);

  CHECK(out == "");
}

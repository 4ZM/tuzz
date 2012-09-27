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
  std::string out;

  auto fijn = std::make_shared<tuzz::transform_finjector>([] (char x) { return x; });
  fijn->inject(text.cbegin(), text.cend(), std::back_inserter(out));

  CHECK(out == text);
}

TEST_CASE( "tuzz/finjectors/transform_finjectors/external_f", "Testing using the toupper function" ) {

  const std::string text("fUbAr");
  std::string out;

  auto fijn = std::make_shared<tuzz::transform_finjector>(toupper);
  fijn->inject(text.cbegin(), text.cend(), std::back_inserter(out));

  CHECK(out == "FUBAR");
}

TEST_CASE( "tuzz/finjectors/transform_finjectors/empty_input", "Runing on empty input" ) {

  const std::string text;
  std::string out;

  auto fijn = std::make_shared<tuzz::transform_finjector>(toupper);
  fijn->inject(text.cbegin(), text.cend(), std::back_inserter(out));

  CHECK(out == "");
}

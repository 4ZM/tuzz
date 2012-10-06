#include "catch.hpp"

#include "tuzz/finjectors/insert_finjector.hpp"

#include <memory>
#include <string.h>

using namespace tuzz;

TEST_CASE( "tuzz/finjectors/insert_finjector/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::insert_finjector finj("!", 1, position::begining));
}

TEST_CASE( "tuzz/finjectors/insert_finjector/once", "One character repetition" ) {

  SECTION("first", "Repeat first position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::begining);
    std::string out = fijn->inject(text);

    CHECK(out == "!abc");
  }
  SECTION("mid", "Repeat mid position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::middle);
    std::string out = fijn->inject(text);

    CHECK(out == "a!bc");
  }
  SECTION("last", "Repeat last position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::end);
    std::string out = fijn->inject(text);

    CHECK(out == "abc!");
  }
}

TEST_CASE( "tuzz/finjectors/insert_finjector/empty", "Empty input" ) {

  SECTION("first", "Repeat first position") {
    const std::string text("");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::begining);
    std::string out = fijn->inject(text);

    CHECK(out == "!");
  }
  SECTION("mid", "Repeat mid position") {
    const std::string text("");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::middle);
    std::string out = fijn->inject(text);

    CHECK(out == "!");
  }
  SECTION("last", "Repeat last position") {
    std::string text("");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::end);
    std::string out = fijn->inject(text);

    CHECK(out == "!");
  }
}

TEST_CASE( "tuzz/finjectors/insert_finjector/poscombo", "Combining positions" ) {

  SECTION("firstlast", "Repeat first and last position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::begining | position::end);
    std::string out = fijn->inject(text);

    CHECK(out == "!abc!");
  }
}

TEST_CASE( "tuzz/finjectors/insert_finjector/wspadding", "Input contains white spece" ) {

  SECTION("first, no strip", "Repeat first position without removing the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::begining);
    std::string out = fijn->inject(text);

    CHECK(out == "! \tfubar");
  }
  SECTION("first, explicit no strip", "Repeat first position without removing the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::begining, false);
    std::string out = fijn->inject(text);

    CHECK(out == "! \tfubar");
  }
  SECTION("first, strip", "Repeat first position, removing the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::insert_finjector>("!", 1, position::begining, true);
    std::string out = fijn->inject(text);

    CHECK(out == " \t!fubar");
  }
}

TEST_CASE( "tuzz/finjectors/insert_finjector/longinsert", "Insert a string" ) {

  const std::string text("fubar");
  
  auto fijn = std::make_shared<tuzz::insert_finjector>("baaz", 2, position::begining);
  std::string out = fijn->inject(text);
  
  CHECK(out == "baazbaazfubar");
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/finjectors/repeat_finjector.hpp"

#include <memory>
#include <string.h>

TEST_CASE( "tuzz/finjectors/repeat_finjector/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::repeat_finjector finj(1, position::begining));
}

TEST_CASE( "tuzz/finjectors/single_finjectors/once", "One character repetition" ) {

  SECTION("first", "Repeat first position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining);
    std::string out = fijn->inject(text);

    CHECK(out == "aabc");
  }
  SECTION("mid", "Repeat first position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::middle);
    std::string out = fijn->inject(text);

    CHECK(out == "abbc");
  }
  SECTION("last", "Repeat first position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::end);
    std::string out = fijn->inject(text);

    CHECK(out == "abcc");
  }
}

TEST_CASE( "tuzz/finjectors/single_finjectors/empty", "Empty input" ) {

  SECTION("first", "Repeat first position") {
    const std::string text("");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining);
    std::string out = fijn->inject(text);

    CHECK(out == text);
  }
  SECTION("mid", "Repeat first position") {
    const std::string text("");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::middle);
    std::string out = fijn->inject(text);

    CHECK(out == text);
  }
  SECTION("last", "Repeat first position") {
    std::string text("");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::end);
    std::string out = fijn->inject(text);

    CHECK(out == text);
  }
}

TEST_CASE( "tuzz/finjectors/single_finjectors/poscombo", "Combining positions" ) {

  SECTION("first", "Repeat first and last position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining | position::end);
    std::string out = fijn->inject(text);

    CHECK(out == "aabcc");
  }
  SECTION("mid", "Repeat first and middle position") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining | position::middle);
    std::string out = fijn->inject(text);

    CHECK(out == "aabbc");
  }
  SECTION("last", "Repeat all positions") {
    const std::string text("abc");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining | position::middle | position::end);
    std::string out = fijn->inject(text);

    CHECK(out == "aabbcc");
  }
}

TEST_CASE( "tuzz/finjectors/single_finjectors/wspadding", "Input contains white spece" ) {

  SECTION("first, no strip", "Repeat first position without removing the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining);
    std::string out = fijn->inject(text);

    CHECK(out == "  \tfubar");
  }
  SECTION("first, explicit no strip", "Repeat first position without removing the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining, false);
    std::string out = fijn->inject(text);

    CHECK(out == "  \tfubar");
  }
  SECTION("first, strip", "Repeat first position, removing the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining, true);
    std::string out = fijn->inject(text);

    CHECK(out == " \tffubar");
  }
  SECTION("last, no strip", "Repeat last position, don't touch the whitespace") {
    const std::string text(" \tfubar");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::end, true);
    std::string out = fijn->inject(text);

    CHECK(out == " \tfubarr");
  }
  SECTION("fist & last, strip", "Repeat first and last position, strip ws") {
    const std::string text(" \tfubar\n ");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::begining | position::end, true);
    std::string out = fijn->inject(text);

    CHECK(out == " \tffubarr\n ");
  }
  SECTION("fist & last, strip, mid rep", "Repeat middle position, strip ws") {
    const std::string text("     bar ");

    auto fijn = std::make_shared<tuzz::repeat_finjector>(1, position::middle, true);
    std::string out = fijn->inject(text);

    CHECK(out == "     baar ");
  }
}

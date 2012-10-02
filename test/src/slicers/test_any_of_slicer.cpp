#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/slicers/any_of_slicer.hpp"

TEST_CASE( "tuzz/slicers/any_of_slicer/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::any_of_slicer(""));
  REQUIRE_NOTHROW(tuzz::any_of_slicer("fubar"));

  // WHY!?
  // std::string str("fubar");
  // REQUIRE_NOTHROW(tuzz::any_of_slicer(str));
  REQUIRE_NOTHROW(tuzz::any_of_slicer(std::string("fubar")));
}

TEST_CASE( "tuzz/slicers/any_of_slicer/delimiter", "Slicing with a single delimiter" ) {
  const std::string text("fu,bar,baz");

  tuzz::any_of_slicer aos(",");
  std::vector<tuzz::chunk> chunks = aos.slice(text);

  REQUIRE(chunks.size() == 3);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
  CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "bar");
  CHECK(std::string(chunks[2].cbegin(), chunks[2].cend()) == "baz");
}

TEST_CASE( "tuzz/slicers/any_of_slicer/delimiters", "Slicing with multiple delimiters" ) {
  const std::string text("fu,bar;baz");

  tuzz::any_of_slicer aos(";,");
  std::vector<tuzz::chunk> chunks = aos.slice(text);

  REQUIRE(chunks.size() == 3);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
  CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "bar");
  CHECK(std::string(chunks[2].cbegin(), chunks[2].cend()) == "baz");
}

TEST_CASE( "tuzz/slicers/any_of_slicer/no_match", "Slicing without matching delimiters" ) {
  const std::string text("fu,bar;baz");

  tuzz::any_of_slicer aos(".-");
  std::vector<tuzz::chunk> chunks = aos.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == text);
}

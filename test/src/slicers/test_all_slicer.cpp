#include "catch.hpp"

#include "tuzz/slicers/all_slicer.hpp"

TEST_CASE( "tuzz/slicers/all_slicer/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::all_slicer as);
}

TEST_CASE( "tuzz/slicers/all_slicer/vanilla", "Testing the minimal slicer" ) {
  const std::string text("fubar");
  tuzz::all_slicer as;
  std::vector<tuzz::chunk> chunks = as.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fubar");
}

TEST_CASE( "tuzz/slicers/all_slicer/empty", "Empty input" ) {
  const std::string text;
  tuzz::all_slicer as;
  std::vector<tuzz::chunk> chunks = as.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "");
}

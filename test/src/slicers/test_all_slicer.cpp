#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/slicers/all_slicer.hpp"

TEST_CASE( "tuzz/slicers/all_slicer/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::all_slicer<std::string::const_iterator> as);
}

TEST_CASE( "tuzz/slicers/all_slicer/vanilla", "Testing the minimal slicer" ) {
  std::string text("fubar");
  tuzz::all_slicer<std::string::const_iterator> as;
  tuzz::chunks<std::string::const_iterator> chunks = as.slice(text.cbegin(), text.cend());

  REQUIRE(chunks.size() == 1);
  CHECK(std::string(chunks[0].first, chunks[0].second) == "fubar");
}

TEST_CASE( "tuzz/slicers/all_slicer/empty", "Empty input" ) {
  std::string text;
  tuzz::all_slicer<std::string::const_iterator> as;
  tuzz::chunks<std::string::const_iterator> chunks = as.slice(text.cbegin(), text.cend());

  REQUIRE(chunks.size() == 1);
  CHECK(std::string(chunks[0].first, chunks[0].second) == "");
}

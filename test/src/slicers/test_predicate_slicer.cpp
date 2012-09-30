#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/slicers/predicate_slicer.hpp"

TEST_CASE( "tuzz/slicers/predicate_slicer/ctor", "Testing the constructor" ) {
  REQUIRE_NOTHROW(tuzz::predicate_slicer ps([] (char) { return true; }));
}

TEST_CASE( "tuzz/slicers/predicate_slicer/delimiter", "Slicing with delimiter" ) {
  const std::string text("fu,bar,baz");

  tuzz::predicate_slicer ps([] (char c) { return c == ','; });
  std::vector<tuzz::chunk> chunks = ps.slice(text);

  REQUIRE(chunks.size() == 3);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
  CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "bar");
  CHECK(std::string(chunks[2].cbegin(), chunks[2].cend()) == "baz");
}

TEST_CASE( "tuzz/slicers/predicate_slicer/all_none", "Slice on all or none of the characters" ) {
  const std::string text("fubar");

  SECTION("all", "All characters") {
    tuzz::predicate_slicer all([] (char c) { return true; });
    std::vector<tuzz::chunk> chunks = all.slice(text);

    REQUIRE(chunks.size() == 6);
    CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "");
    CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "");
    CHECK(std::string(chunks[2].cbegin(), chunks[2].cend()) == "");
    CHECK(std::string(chunks[3].cbegin(), chunks[3].cend()) == "");
    CHECK(std::string(chunks[4].cbegin(), chunks[4].cend()) == "");
    CHECK(std::string(chunks[5].cbegin(), chunks[5].cend()) == "");
  }

  SECTION("none", "None of the characters") {
    tuzz::predicate_slicer none([] (char c) { return false; });
    std::vector<tuzz::chunk> chunks = none.slice(text);
    REQUIRE(chunks.size() == 1);
    CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fubar");
  }
}

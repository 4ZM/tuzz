#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/slicers/delimiter_slicer.hpp"

#include <cstring>

TEST_CASE( "tuzz/slicers/delimiter_slicer/ctor", "Construction" ) {
  CHECK_NOTHROW(tuzz::delimiter_slicer ds('\0'));
  CHECK_NOTHROW(tuzz::delimiter_slicer ds(','));
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/vanilla", "Slice a text with delimiters" ) {
  const std::string text("fu,bar;baz");
  tuzz::delimiter_slicer ds(',');
  tuzz::chunks chunks = ds.slice(text);

  REQUIRE(chunks.size() == 2);
  CHECK(std::string(chunks[0].first, chunks[0].second) == "fu");
  CHECK(std::string(chunks[1].first, chunks[1].second) == "bar;baz");
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/undelimited", "Slice a text without delimiters" ) {
  const std::string text("fu");
  tuzz::delimiter_slicer ds(',');
  tuzz::chunks chunks = ds.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(chunks[0].first == text.cbegin());
  CHECK(chunks[0].second == text.cend());
  CHECK(std::string(chunks[0].first, chunks[0].second) == "fu");
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/empty", "Slice an empty text" ) {
  std::string text;
  tuzz::delimiter_slicer ds(',');
  tuzz::chunks chunks = ds.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(chunks[0].first == text.cbegin());
  CHECK(chunks[0].second == text.cend());
  CHECK(std::string(chunks[0].first, chunks[0].second) == "");
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/ends", "The delimiter at the begining/end of the text" ) {
  SECTION("begining", "Delimiter at the front of the text") {
    const std::string text(",fu");
    tuzz::delimiter_slicer ds(',');
    tuzz::chunks chunks = ds.slice(text);

    REQUIRE(chunks.size() == 2);

    // begin == end, i.e. empty chunk
    {
      CHECK(chunks[0].first == text.cbegin());
      CHECK(chunks[0].second == text.cbegin());
      CHECK(std::string(chunks[0].first, chunks[0].second) == "");
    }

    // begin != end, "fu"
    {
      CHECK(chunks[1].first == text.cbegin() + 1);
      CHECK(chunks[1].second == text.cend());
      CHECK(std::string(chunks[1].first, chunks[1].second) == "fu");
    }
  }
  SECTION("end", "Delimiter at the end of the text") {
    const std::string text("fu,");
    tuzz::delimiter_slicer ds(',');
    tuzz::chunks chunks = ds.slice(text);

    REQUIRE(chunks.size() == 2);

    // begin != end, "fu"
    {
      CHECK(chunks[0].first == text.cbegin());
      CHECK(chunks[0].second == text.cbegin() + 2);
      CHECK(std::string(chunks[0].first, chunks[0].second) == "fu");
    }

    // begin == end, i.e. empty chunk
    {
      CHECK(chunks[1].first == text.cend());
      CHECK(chunks[1].second == text.cend());
      CHECK(std::string(chunks[1].first, chunks[1].second) == "");
    }
  }
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/emptychunks", "The multiple delimiters without chunk material") {
  const std::string text("fu,,,bar");
  tuzz::delimiter_slicer ds(',');
  tuzz::chunks chunks = ds.slice(text);

  // {"fu", "", "", "bar"}
  REQUIRE(chunks.size() == 4);
  CHECK(std::string(chunks[0].first, chunks[0].second) == "fu");
  CHECK(std::string(chunks[1].first, chunks[1].second) == "");
  CHECK(std::string(chunks[2].first, chunks[2].second) == "");
  CHECK(std::string(chunks[3].first, chunks[3].second) == "bar");

  CHECK(chunks[1].first < chunks[2].first);
}

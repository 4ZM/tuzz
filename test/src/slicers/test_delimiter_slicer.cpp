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
  std::vector<tuzz::chunk> chunks = ds.slice(text);

  REQUIRE(chunks.size() == 2);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
  CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "bar;baz");
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/undelimited", "Slice a text without delimiters" ) {
  const std::string text("fu");
  tuzz::delimiter_slicer ds(',');
  std::vector<tuzz::chunk> chunks = ds.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(chunks[0].cbegin() == text.cbegin());
  CHECK(chunks[0].cend() == text.cend());
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/empty", "Slice an empty text" ) {
  std::string text;
  tuzz::delimiter_slicer ds(',');
  std::vector<tuzz::chunk> chunks = ds.slice(text);

  REQUIRE(chunks.size() == 1);
  CHECK(chunks[0].cbegin() == text.cbegin());
  CHECK(chunks[0].cend() == text.cend());
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "");
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/ends", "The delimiter at the begining/end of the text" ) {
  SECTION("begining", "Delimiter at the front of the text") {
    const std::string text(",fu");
    tuzz::delimiter_slicer ds(',');
    std::vector<tuzz::chunk> chunks = ds.slice(text);

    REQUIRE(chunks.size() == 2);

    // begin == end, i.e. empty chunk
    {
      CHECK(chunks[0].cbegin() == text.cbegin());
      CHECK(chunks[0].cend() == text.cbegin());
      CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "");
    }

    // begin != end, "fu"
    {
      CHECK(chunks[1].cbegin() == text.cbegin() + 1);
      CHECK(chunks[1].cend() == text.cend());
      CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "fu");
    }
  }
  SECTION("end", "Delimiter at the end of the text") {
    const std::string text("fu,");
    tuzz::delimiter_slicer ds(',');
    std::vector<tuzz::chunk> chunks = ds.slice(text);

    REQUIRE(chunks.size() == 2);

    // begin != end, "fu"
    {
      CHECK(chunks[0].cbegin() == text.cbegin());
      CHECK(chunks[0].cend() == text.cbegin() + 2);
      CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
    }

    // begin == end, i.e. empty chunk
    {
      CHECK(chunks[1].cbegin() == text.cend());
      CHECK(chunks[1].cend() == text.cend());
      CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "");
    }
  }
}

TEST_CASE( "tuzz/slicers/delimiter_slicer/emptychunks", "The multiple delimiters without chunk material") {
  const std::string text("fu,,,bar");
  tuzz::delimiter_slicer ds(',');
  std::vector<tuzz::chunk> chunks = ds.slice(text);

  // {"fu", "", "", "bar"}
  REQUIRE(chunks.size() == 4);
  CHECK(std::string(chunks[0].cbegin(), chunks[0].cend()) == "fu");
  CHECK(std::string(chunks[1].cbegin(), chunks[1].cend()) == "");
  CHECK(std::string(chunks[2].cbegin(), chunks[2].cend()) == "");
  CHECK(std::string(chunks[3].cbegin(), chunks[3].cend()) == "bar");

  CHECK(chunks[1].cbegin() < chunks[2].cbegin());
}

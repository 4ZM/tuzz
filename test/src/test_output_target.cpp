#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/output_target.hpp"
#include "tuzz/numbered_string.hpp"

TEST_CASE( "tuzz/io/output_dst/ctor", "Construction" ) {
  // Default
  REQUIRE_NOTHROW(tuzz::output_target());

  // Copy ctor
  tuzz::numbered_string ns("%2n");
  REQUIRE_NOTHROW(tuzz::output_target(ns));

  // Move ctor
  REQUIRE_NOTHROW(tuzz::output_target(tuzz::numbered_string("%2n")));
}


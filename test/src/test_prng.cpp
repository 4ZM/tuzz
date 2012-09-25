#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/prng.hpp"

#include <algorithm>
#include <utility>

TEST_CASE( "tuzz/io/prng/ctor", "Construction" ) {
  // Random seed
  REQUIRE_NOTHROW(tuzz::prng());

  // Fixed seed
  REQUIRE_NOTHROW(tuzz::prng(100));
}

TEST_CASE( "tuzz/io/prng/gen_rseed", "Generate some numbers in [0,1337] using random seed" ) {
  tuzz::prng gen1;
  std::vector<unsigned int> first_pass(100);
  std::generate(first_pass.begin(), first_pass.end(), [&] () { return gen1(1337); });

  tuzz::prng gen2;
  std::vector<unsigned int> second_pass(100);
  std::generate(second_pass.begin(), second_pass.end(), [&] () { return gen2(1337); });

  auto res = std::mismatch(first_pass.cbegin(), first_pass.cend(), second_pass.cbegin());

  // There should be some mismatch (with very large probability)
  CHECK_FALSE(res.first == first_pass.cend());

  auto minmax = std::minmax_element(first_pass.cbegin(), first_pass.cend());
  CHECK(*(minmax.first) < *(minmax.second));
  CHECK(*(minmax.first) >= 0);
  CHECK(*(minmax.second) <= 1337);
}

TEST_CASE( "tuzz/io/prng/gen_fseed", "Generate some numbers in [0,1337] using fix seed" ) {
  tuzz::prng gen1(4711);
  std::vector<unsigned int> first_pass(100);
  std::generate(first_pass.begin(), first_pass.end(), [&] () { return gen1(1337); });

  tuzz::prng gen2(4711);
  std::vector<unsigned int> second_pass(100);
  std::generate(second_pass.begin(), second_pass.end(), [&] () { return gen2(1337); });

  auto res = std::mismatch(first_pass.cbegin(), first_pass.cend(), second_pass.cbegin());

  // There should not be any mismatch (same seed)
  CHECK(res.first == first_pass.cend());

  auto minmax = std::minmax_element(first_pass.cbegin(), first_pass.cend());
  CHECK(*(minmax.first) < *(minmax.second));
  CHECK(*(minmax.first) >= 0);
  CHECK(*(minmax.second) <= 1337);
}

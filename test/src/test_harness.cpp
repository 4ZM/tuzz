#include "catch.hpp"

#include "tuzz/harness.hpp"

TEST_CASE( "tuzz/harness/ctor", "Construction" ) {
  REQUIRE_NOTHROW(tuzz::harness("fu", { "baz", "bar" }));
  REQUIRE_NOTHROW(tuzz::harness("foo", { "baz" }, { "boo" }));
}

TEST_CASE( "tuzz/harness/badtarget", "Non existent target" ) {
	const std::string test_prog = "this_is_probably_not_a_valid_binary_name";
	tuzz::harness h(test_prog, { test_prog });
	REQUIRE_THROWS_AS(h.start(), tuzz::harness_error);
}

TEST_CASE( "tuzz/harness/exitcodes", "Exit codes" ) {
	const std::string test_prog = "./test/targets/exits";
	tuzz::harness h(test_prog, { test_prog, "0" }, { });

	h.start();

	// wait for it, then assert on return value
}

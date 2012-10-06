#include "catch.hpp"

#include "tuzz/numbered_string.hpp"

TEST_CASE( "tuzz/utility/numbered_string/ctor", "Construction" ) {
	REQUIRE_THROWS_AS(tuzz::numbered_string(""), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu"), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu%"), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu%4"), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu%-2n"), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu%0x2n"), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu%0n"), tuzz::numbered_string_format_error);
	REQUIRE_THROWS_AS(tuzz::numbered_string("fu%20n"), tuzz::numbered_string_format_error);

	REQUIRE_NOTHROW(tuzz::numbered_string ns("%n"));
	REQUIRE_NOTHROW(tuzz::numbered_string ns("%1n"));
	REQUIRE_NOTHROW(tuzz::numbered_string ns("%5n"));
	REQUIRE_NOTHROW(tuzz::numbered_string ns("a%nb"));
	REQUIRE_NOTHROW(tuzz::numbered_string ns("a%3nb"));
}

TEST_CASE( "tuzz/utility/numbered_string/str", "String creation" ) {
	SECTION("%2n", "At least two digits") {
		tuzz::numbered_string ns("%2n");
		CHECK( ns.str(0) == "00" );
		CHECK( ns.str(1) == "01" );
		CHECK( ns.str(12) == "12" );
		CHECK( ns.str(123) == "123" );
	}

	SECTION("fu%2n", "fu + at least two digits") {
		tuzz::numbered_string ns("fu%2n");
		CHECK( ns.str(0) == "fu00" );
		CHECK( ns.str(1) == "fu01" );
		CHECK( ns.str(12) == "fu12" );
		CHECK( ns.str(123) == "fu123" );
	}

	SECTION("%2nbar", "At least two digits + bar") {
		tuzz::numbered_string ns("%2nbar");
		CHECK( ns.str(0) == "00bar" );
		CHECK( ns.str(1) == "01bar" );
		CHECK( ns.str(12) == "12bar" );
		CHECK( ns.str(123) == "123bar" );
	}

	SECTION("fu%2nbar", "fu + at least two digits + bar") {
		tuzz::numbered_string ns("fu%2nbar");
		CHECK( ns.str(0) == "fu00bar" );
		CHECK( ns.str(1) == "fu01bar" );
		CHECK( ns.str(12) == "fu12bar" );
		CHECK( ns.str(123) == "fu123bar" );
	}
}


TEST_CASE( "tuzz/utility/numbered_string/nonalpha", "Non alpha characters" ) {
	REQUIRE_NOTHROW(tuzz::numbered_string ns(R"(!@#$^&*()+{}[]_%n_<>,."'\|/)"));
}

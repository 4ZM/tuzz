#include "catch.hpp"

#include "tuzz/input_source.hpp"

TEST_CASE( "tuzz/io/input_src/ctor", "Construction" ) {
	REQUIRE_NOTHROW(tuzz::input_source());
	REQUIRE_THROWS_AS(tuzz::input_source("/a/file/that/probably/doesnt/exist"), tuzz::input_source_error);
	REQUIRE_THROWS_AS(tuzz::input_source(R"(!@#$%^&*()_+='\)"), tuzz::input_source_error);
}

TEST_CASE( "tuzz/io/input_source/mov", "Move constructor and assignment" ) {
	tuzz::input_source src;
	REQUIRE(src.get_stream().get() == &std::cin);
	
	tuzz::input_source assigned_src = tuzz::input_source();
	CHECK(assigned_src.get_stream().get() == &std::cin);
}

TEST_CASE( "tuzz/io/input_source/file", "Using a file for input" ) {
	REQUIRE_NOTHROW(tuzz::input_source("test/data/empty"));
		
	tuzz::input_source src("test/data/empty");
	CHECK_FALSE(src.get_stream().get() ==  &std::cin);
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/input_src.hpp"

TEST_CASE( "tuzz/io/input_src/ctor", "Construction" ) {
	REQUIRE_NOTHROW(tuzz::input_src());
	REQUIRE_THROWS_AS(tuzz::input_src("/a/file/that/probably/doesnt/exist"), tuzz::input_src_error);
	REQUIRE_THROWS_AS(tuzz::input_src(R"(!@#$%^&*()_+='\)"), tuzz::input_src_error);
}

TEST_CASE( "tuzz/io/input_src/mov", "Move constructor and assignment" ) {
	tuzz::input_src src;
	REQUIRE(src.input_from_stdin());
	REQUIRE(&src.get_src() == &std::cin);

	tuzz::input_src moved_to_src(std::move(src));
	//tuzz::input_src moved_to_src(tuzz::input_src());
	CHECK(moved_to_src.input_from_stdin());
	CHECK(&moved_to_src.get_src() == &std::cin);
	
	tuzz::input_src assigned_src = tuzz::input_src();
	CHECK(assigned_src.input_from_stdin());
	CHECK(&assigned_src.get_src() == &std::cin);
}

TEST_CASE( "tuzz/io/input_src/file", "Using a file for input" ) {
	REQUIRE_NOTHROW(tuzz::input_src("test/data/empty"));
		
	tuzz::input_src src("test/data/empty");
	CHECK(src.input_from_stdin() == false);
	CHECK_FALSE(&src.get_src() ==  &std::cin);
}

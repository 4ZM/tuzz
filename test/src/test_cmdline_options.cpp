#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "tuzz/cmdline_options.hpp"

#include <sstream>

TEST_CASE( "tuzz/utility/options/noargs", "No aruments" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(1, argv, ss));
	CHECK(ss.str().empty());

	{
		tuzz::cmdline_options co(1, argv, ss);
		CHECK(co.input_from_stdin());
		CHECK(co.output_to_stdout());
		CHECK(co.get_input_specification() == "-");
		CHECK(co.get_output_specification() == "-");
		CHECK_FALSE(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/help", "Help request" ) {
	{
		std::stringstream ss;
		const char* argv[] = { "prog_name", "-h", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
		CHECK_FALSE(ss.str().empty());
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.termination_requested());
	}
	{
		std::stringstream ss;
		const char* argv[] = { "prog_name", "--help", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
		CHECK_FALSE(ss.str().empty());
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/version", "Version info request" ) {
	{
		std::stringstream ss;
		const char* argv[] = { "prog_name", "-v", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
		CHECK_FALSE(ss.str().empty());
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.termination_requested());
	}
	{
		std::stringstream ss;
		const char* argv[] = { "prog_name", "--version", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
		CHECK_FALSE(ss.str().empty());
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/positional_input", "Input specified as positional argument" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", "file", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
	CHECK(ss.str().empty());

	{
		tuzz::cmdline_options co(2, argv, ss);
		CHECK_FALSE(co.input_from_stdin());
		CHECK(co.output_to_stdout());
		CHECK(co.get_input_specification() == "file");
		CHECK(co.get_output_specification() == "-");
		CHECK_FALSE(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/explicit_input", "Input specified with -i option" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", "-i", "file", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(3, argv, ss));
	CHECK(ss.str().empty());

	{
		tuzz::cmdline_options co(3, argv, ss);
		CHECK_FALSE(co.input_from_stdin());
		CHECK(co.output_to_stdout());
		CHECK(co.get_input_specification() == "file");
		CHECK(co.get_output_specification() == "-");
		CHECK_FALSE(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/explicit_stdin", "Input specified with -i- option" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", "-i-", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
	CHECK(ss.str().empty());

	{
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.input_from_stdin());
		CHECK(co.output_to_stdout());
		CHECK(co.get_input_specification() == "-");
		CHECK(co.get_output_specification() == "-");
		CHECK_FALSE(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/explicit_output", "Output specified with -o option" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", "-o", "outspec", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(3, argv, ss));
	CHECK(ss.str().empty());

	{
		tuzz::cmdline_options co(3, argv, ss);
		CHECK(co.input_from_stdin());
		CHECK_FALSE(co.output_to_stdout());
		CHECK(co.get_input_specification() == "-");
		CHECK(co.get_output_specification() == "outspec");
		CHECK_FALSE(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/explicit_stdout", "Output specified with -o- option" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", "-o-", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
	CHECK(ss.str().empty());

	{
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.input_from_stdin());
		CHECK(co.output_to_stdout());
		CHECK(co.get_input_specification() == "-");
		CHECK(co.get_output_specification() == "-");
		CHECK_FALSE(co.termination_requested());
	}
}

TEST_CASE( "tuzz/utility/options/both_in_out", "Mix variations of -i and -o options" ) {
	SECTION("-i-/-o-", "Using stdin and stdout") {
		std::stringstream ss;
		const char* argv[] = { "prog_name", "-o", "-", "-i", "-", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(5, argv, ss));
		CHECK(ss.str().empty());
		
		{
			tuzz::cmdline_options co(5, argv, ss);
			CHECK(co.input_from_stdin());
			CHECK(co.output_to_stdout());
			CHECK(co.get_input_specification() == "-");
			CHECK(co.get_output_specification() == "-");
			CHECK_FALSE(co.termination_requested());
		}
	}

	SECTION("-i-/-oout", "Using stdin and output specification") {
		std::stringstream ss;
		const char* argv[] = { "prog_name", "-o", "out", "-i", "-", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(5, argv, ss));
		CHECK(ss.str().empty());
		
		{
			tuzz::cmdline_options co(5, argv, ss);
			CHECK(co.input_from_stdin());
			CHECK_FALSE(co.output_to_stdout());
			CHECK(co.get_input_specification() == "-");
			CHECK(co.get_output_specification() == "out");
			CHECK_FALSE(co.termination_requested());
		}
	}
	
	SECTION("-iin/-o-", "Using input specification and stdout") {
		std::stringstream ss;
		const char* argv[] = { "prog_name", "-o", "-", "-i", "in", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(5, argv, ss));
		CHECK(ss.str().empty());
		
		{
			tuzz::cmdline_options co(5, argv, ss);
			CHECK_FALSE(co.input_from_stdin());
			CHECK(co.output_to_stdout());
			CHECK(co.get_input_specification() == "in");
			CHECK(co.get_output_specification() == "-");
			CHECK_FALSE(co.termination_requested());
		}
	}

	SECTION("-iin/-oout", "Using input and output specifications") {
		std::stringstream ss;
		const char* argv[] = { "prog_name", "-o", "out", "-i", "in", nullptr };
		REQUIRE_NOTHROW(tuzz::cmdline_options(5, argv, ss));
		CHECK(ss.str().empty());
		
		{
			tuzz::cmdline_options co(5, argv, ss);
			CHECK_FALSE(co.input_from_stdin());
			CHECK_FALSE(co.output_to_stdout());
			CHECK(co.get_input_specification() == "in");
			CHECK(co.get_output_specification() == "out");
			CHECK_FALSE(co.termination_requested());
		}
	}	
}

TEST_CASE( "tuzz/utility/options/badargs", "Invalid arguments" ) {
	std::stringstream ss;
	const char* argv[] = { "prog_name", "--invalidarg", nullptr };
	REQUIRE_NOTHROW(tuzz::cmdline_options(2, argv, ss));
	CHECK_FALSE(ss.str().empty());

	{
		tuzz::cmdline_options co(2, argv, ss);
		CHECK(co.termination_requested());
	}
}

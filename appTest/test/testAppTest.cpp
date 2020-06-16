//
// Created by hdaniel on 16/06/2020.
//

#include <catch2/catch.hpp>
#include <sstream>
#include <iostream>
#include "../AppTest.hpp"

using std::string;
using namespace had;

string errorUsage("Usage: executable <argument>\n");

//app main function.
int appmain(int argc, char* argv[], std::istream& in, std::ostream& out) {

	if (argc != 2) {
		out << errorUsage;
		return -1;
	}

	char s[128];
	in.getline(s, 128);
	out << s << argv[1] << std::endl;
	return 0;
}

TEST_CASE( "Application test utilities", "[AppTest]" ) {
	AppTest apptest;

	constexpr const char* input = "anInput.";
	constexpr const char* arg = " anArg.";
	string outputOK(string(input) + arg +'\n');

	const char* appPath = "./executableApp";
	AppTest::cmdlineargs errorNumArgs = { appPath };
	AppTest::cmdlineargs okArgs = { appPath, arg };

	SECTION("test") {

		apptest.clearOUTstream();
		apptest.exec(appmain, errorNumArgs);
		REQUIRE(apptest.outStream() == errorUsage);

		apptest.inStream(input);
		apptest.clearOUTstream();
		apptest.exec(appmain, okArgs);
		REQUIRE(apptest.outStream() == outputOK);
	}
}

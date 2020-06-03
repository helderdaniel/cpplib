//
// Created by hdaniel on 22/05/2020.
//

#include <catch2/catch.hpp>
#include <sstream>
#include "../String.hpp"

using namespace  had;

TEST_CASE( "String utilities", "[String]" ) {

	const int cases = 6;
	const std::string str[] = {
			"",
			"../test/samples/files/",
			};

	const std::string bdel[] = { "/", "/", "t/s", "/", "__NotExixting__", "/", "/" };
	const std::string edel[] = { "/", "/", "s/",  "__NotExixting__", "/", "/", "/" };

	SECTION("First") {
		int idx;
		const int pos[] = { 0, 4, 0, 0, 0, 99999 };

		std::string outputExpected[] = {
				"test",
				"samples",
				"ample",
				"",
				"",
				"",
		};
		const int idxExpected[] = { 8, 16, 16, -1, -1, -1 };

		std::string output = "just to make sure it is cleaned";

		//Null
		idx = String::firstSubstring(str[0], bdel[0], edel[0], output, pos[0]);
		REQUIRE(output == outputExpected[4]);
		REQUIRE(idx == idxExpected[4]);
		idx = String::firstSubstring("", "", "", output, 0);
		REQUIRE(output == outputExpected[4]);
		REQUIRE(idx == idxExpected[4]);
		idx = String::firstSubstring(str[0], bdel[0], edel[0], output, pos[5]);
		REQUIRE(output == outputExpected[4]);
		REQUIRE(idx == idxExpected[4]);

		for (int i = 0; i < cases; ++i) {
			idx = String::firstSubstring(str[1], bdel[i], edel[i], output, pos[i]);
			REQUIRE(output == outputExpected[i]);
			REQUIRE(idx == idxExpected[i]);
		}
	}


	SECTION("Last") {
		int idx;
		const int pos[] = {999999, 18, 18, 0, 0, 0};

		std::string outputExpected[] = {
				"files",
				"samples",
				"ample",
				"",
				"",
				"",
		};
		const int idxExpected[] = {14, 6, 5, -1, -1, -1};

		std::string output = "just to make sure it is cleaned";

		//Null
		idx = String::lastSubstring(str[0], bdel[0], edel[0], output, pos[0]);
		REQUIRE(output == outputExpected[4]);
		REQUIRE(idx == idxExpected[4]);
		idx = String::lastSubstring("", "", "", output, 0);
		REQUIRE(output == outputExpected[4]);
		REQUIRE(idx == idxExpected[4]);
		idx = String::lastSubstring(str[0], bdel[0], edel[0], output, pos[5]);
		REQUIRE(output == outputExpected[4]);
		REQUIRE(idx == idxExpected[4]);

		for (int i = 6; i < cases; ++i) {
			idx = String::lastSubstring(str[1], bdel[i], edel[i], output, pos[i]);
			REQUIRE(output == outputExpected[i]);
			REQUIRE(idx == idxExpected[i]);
		}
	}

	SECTION("rnd") {
		srand(0);
		REQUIRE(String::rand(3) == string("g\306i"));
		REQUIRE(String::randAlphaNum(16) == "7JncCHryDsbzayy4");
	}

}

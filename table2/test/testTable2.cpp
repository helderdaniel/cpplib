//
// Created by hdaniel on 22/05/2020.
//

#include <catch2/catch.hpp>
#include <sstream>
#include "../Table2.hpp"

using namespace had;
using std::string;

TEST_CASE( "Table2", "[Table2]" ) {
	enum Enum { A, B, C, D };

	map<int, string> t0 = {
			{ 1, "One" },
			{3, "Three" },
			{ 4, "Four" }

	};

	map<Enum, string> t1 = {
			{ A, "One" },
			{ C, "Three" },
			{ D, "Four" }

	};


	SECTION("test t0") {
		Table2<int, string> table(t0);

		for (const auto & [key, value] : t0) {
			REQUIRE(table.value(key) == value);
			REQUIRE(table.key(value) == key);		//reverse

		}

		//Non existant
		REQUIRE_THROWS_AS(table.value(2),std::out_of_range);
		REQUIRE_THROWS_AS(table.key(""),std::out_of_range);
	}

	SECTION("test t1") {
		Table2<Enum, string> table(t1);

		for (const auto & [key, value] : t1) {
			REQUIRE(table.value(key) == value);
			REQUIRE(table.key(value) == key);		//reverse
		}

		//Non existant
		REQUIRE_THROWS_AS(table.value(B),std::out_of_range);
		REQUIRE_THROWS_AS(table.key(""),std::out_of_range);
	}

}

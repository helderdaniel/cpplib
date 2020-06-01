//
// Created by hdaniel on 19/05/2020.
//

#include <catch2/catch.hpp>
#include "../File.hpp"

using namespace had;

const std::string path     = "../test/samples/files/";
const std::string file0c   = path + "file0.copy.txt";
const std::string file0ro  = path + "file0.ro.txt";
const std::string file0    = path + "file0.txt";
const std::string file1    = path + "file1.txt";
const std::string file2    = path + "file2.txt";
const std::string file2n   = path + "file2n.txt";
const std::string file3    = path + "file3.txt";
const std::string file3e   = path + "file3e.txt";
const std::string file3n   = path + "file3n.txt";
const std::string file3nes = path + "file3nes.txt";
const std::string file4    = path + "file4.txt";
const std::string fileNE   = "__NotExist__";
const std::string fileNull;

std::string searchPathToString(const std::string& dir, const std::string& regex, const int depth = -1) {
	std::vector<fs::path> v;
	std::string ret;

	if (depth < 0) v = File::search(dir, regex);
	else 		   v = File::search(dir, regex, depth);
	sort(v.begin(), v.end());
	for (const auto& p : v) {
		ret += p;
		ret += '\n';
	}
	return ret;
}


TEST_CASE( "Copy" "[File]" ) {

	//File NOT found
	REQUIRE_THROWS_AS(
			File::copy(file0, fileNull),
			std::runtime_error);
	REQUIRE_THROWS_WITH(File::copy(file0, fileNull), fileNull + " error: 2");
	REQUIRE_THROWS_AS(
			File::copy(fileNull, file0c),
			std::runtime_error);
	REQUIRE_THROWS_WITH(File::copy(fileNull, file0c), fileNull + " error: 2");
	REQUIRE_THROWS_AS(
			File::copy(fileNE, file0c),
			std::runtime_error);
	REQUIRE_THROWS_WITH(File::copy(fileNE, file0c), fileNE + " error: 2");

	//Read only destination
	REQUIRE_THROWS_AS(
			File::copy(file0, file0ro),
			std::runtime_error);
	REQUIRE_THROWS_WITH(File::copy(file0, file0ro), file0ro + " error: 13");


	//OK copy
	File::copy(file0, file0c);
	REQUIRE(File::cmpbin(file0, file0c));
}


TEST_CASE( "Read" "[File]" ) {
	const std::string s3 =
					"one\n"
					"two\n"
					"three\n";

	REQUIRE(File::read(file3) == s3);

	std::ifstream sf (file3);
	REQUIRE(File::read(sf) == s3);
}


TEST_CASE( "Search" "[File]" ) {
	const std::string expA =
						"../test/samples/files/file0.copy.txt\n"
						"../test/samples/files/file0.ro.txt\n"
						"../test/samples/files/file0.txt\n"
				      	"../test/samples/files/file1.txt\n"
		 			  	"../test/samples/files/file2.txt\n"
					  	"../test/samples/files/file2n.txt\n";
	const std::string expB =
						"../test/samples/files/inner/file0.txt\n";
	const std::string expC =
						"../test/samples/files/file3.txt\n"
						"../test/samples/files/file3e.txt\n"
						"../test/samples/files/file3n.txt\n"
						"../test/samples/files/file3nes.txt\n"
						"../test/samples/files/file4.txt\n";

	const int cases = 3;
	const std::string expected[] = {
						"",
						expA + expB,
						expA + expC	+ expB,
						expA + expC };

	const std::string regex[] = {
						"",					//null
						"file[0-2](.*)",	//filenames started with: "file[0-2]"
						"(.*)txt" };        //filenames ended with:   "txt"

	for (int i = 0; i < cases; ++i) {
		auto s = searchPathToString(path, regex[i]);
		REQUIRE(expected[i] == s);
	}

	//Test depth
	std::string s;
	s = searchPathToString(path, regex[2], 0);
	REQUIRE(expected[3] == s);
	s = searchPathToString(path, regex[2], 1);
	REQUIRE(expected[2] == s);
	s = searchPathToString(path, regex[2], 2);
	REQUIRE(expected[2] == s);
	s = searchPathToString(path, regex[2], 3);
	REQUIRE(expected[2] == s);
}


TEST_CASE( "Compare" "[File]" ) {

	SECTION("Not found") {
		REQUIRE_THROWS_AS(
				File::cmpbin(fileNull, file0),
				std::runtime_error);
		REQUIRE_THROWS_WITH(File::cmpbin(fileNull, file0), " error: 2");

		REQUIRE_THROWS_AS(
				File::cmpbin(file0, fileNull),
				std::runtime_error);
		REQUIRE_THROWS_WITH(File::cmpbin(file0, fileNull), " error: 2");

		REQUIRE_THROWS_AS(
				File::cmpbin(fileNull, fileNull),
				std::runtime_error);
		REQUIRE_THROWS_WITH(File::cmpbin(fileNull, fileNull), " error: 2");

		REQUIRE_THROWS_AS(
				File::cmpbin(fileNE, file0),
				std::runtime_error);
		REQUIRE_THROWS_WITH(File::cmpbin(fileNE, file0), fileNE + " error: 2");

		REQUIRE_THROWS_AS(
				File::cmpbin(file0, fileNE),
				std::runtime_error);
		REQUIRE_THROWS_WITH(File::cmpbin(file0, fileNE), fileNE + " error: 2");

		REQUIRE_THROWS_AS(
				File::cmpbin(fileNE, fileNE),
				std::runtime_error);
		REQUIRE_THROWS_WITH(File::cmpbin(fileNE, fileNE), fileNE + " error: 2");
	}

	SECTION("Size") {
		REQUIRE(File::cmpsize(file0, file0));
		REQUIRE(File::cmpsize(file3, file3e));
		REQUIRE(File::cmpsize(file3, file3nes));
		REQUIRE(!File::cmpsize(file0, file1));
		REQUIRE(!File::cmpsize(file2, file2n));
	}

	SECTION("Binary") {
		REQUIRE(File::cmpbin(file0, file0));
		REQUIRE(File::cmpbin(file3, file3e));
		REQUIRE(!File::cmpbin(file0, file1));
		REQUIRE(!File::cmpbin(file2, file2n));
		REQUIRE(!File::cmpbin(file3, file3nes));
	}

	SECTION("Text") {
		const std::string s01 =
							"0:\n"
						   	"one\n";
		const std::string s22n =
							"2:\n"
					       	"\n";
		const std::string s33n =
							"1:\n"
					       	"two\n"
						   	"two!\n";
		const std::string s33nes =
							"1:\n"
							"two\n"
							"txo\n";
		const std::string s3nes3 =
							"1:\n"
							"txo\n"
							"two\n";
		const std::string s13n =
							"1:\n"
						   	"two\n"
		 				   	"2:\n"
						   	"three\n";
		const std::string s34n =
							"0:\n"
						   	"one\n"
						   	"one,\n"
						   	"2:\n"
						   	"three\n"
						   	"three,\n"
						   	"3:\n"
						   	"four\n";
		const std::string s43n =
							"0:\n"
						   	"one,\n"
						   	"one\n"
						   	"2:\n"
						   	"three,\n"
						   	"three\n"
						   	"3:\n"
		 				   	"four\n";

	   	//Equal files
		REQUIRE(File::cmptext(file0, file0).empty());
		REQUIRE(File::cmptext(file3, file3e).empty());

		//Different files
		REQUIRE(File::cmptext(file0, file1) == s01);
		REQUIRE(File::cmptext(file1, file0) == s01);

		REQUIRE(File::cmptext(file2, file2n) == s22n);
		REQUIRE(File::cmptext(file3, file3n) == s33n);

		REQUIRE(File::cmptext(file1, file3) == s13n);
		REQUIRE(File::cmptext(file3, file1) == s13n);

		REQUIRE(File::cmptext(file3, file3nes) == s33nes);
		REQUIRE(File::cmptext(file3nes, file3) == s3nes3);

		//Different files and different report if diff order
		REQUIRE(File::cmptext(file3, file4) == s34n);
		REQUIRE(File::cmptext(file4, file3) == s43n);
	}

	SECTION("Test") {
		const std::string s01 =
				"Expected:\n"
				"Actual:\n"
				"one\n";
		const std::string s10 =
				"Expected:\n"
				"one\n"
				"Actual:\n";
		const std::string s34n =
				"Expected:\n"
				"one\n"
				"two\n"
				"three\n"
				"Actual:\n"
				"one,\n"
				"two\n"
				"three,\n"
				"four\n";
		const std::string s43n =
				"Expected:\n"
				"one,\n"
				"two\n"
				"three,\n"
				"four\n"
				"Actual:\n"
				"one\n"
				"two\n"
				"three\n";
		const std::string s33nes =
				"Expected:\n"
				"one\n"
				"two\n"
				"three\n"
				"Actual:\n"
				"one\n"
				"txo\n"
				"three\n";
		const std::string s3nes3 =
				"Expected:\n"
				"one\n"
				"txo\n"
				"three\n"
				"Actual:\n"
				"one\n"
				"two\n"
				"three\n";

		SECTION("test") {
			//Equal files
			REQUIRE(File::test(file0, file0).empty());
			REQUIRE(File::test(file3, file3e).empty());

			//Different files
			REQUIRE(File::test(file0, file1) == s01);
			REQUIRE(File::test(file1, file0) == s10);

			//Different files and different report if diff order
			REQUIRE(File::test(file3, file3nes) == s33nes);
			REQUIRE(File::test(file3nes, file3) == s3nes3);

			//Different files and different report if diff order
			REQUIRE(File::test(file3, file4) == s34n);
			REQUIRE(File::test(file4, file3) == s43n);
		}

		SECTION("teststr") {
			//Equal files
			REQUIRE(File::teststr(file0, File::read(file0)).empty());
			REQUIRE(File::teststr(file3, File::read(file3e)).empty());

			//Different files
			REQUIRE(File::teststr(file0, File::read(file1)) == s01);
			REQUIRE(File::teststr(file1, File::read(file0)) == s10);

			//Different files and different report if diff order
			REQUIRE(File::teststr(file3, File::read(file3nes)) == s33nes);
			REQUIRE(File::teststr(file3nes, File::read(file3)) == s3nes3);

			//Different files and different report if diff order
			REQUIRE(File::teststr(file3, File::read(file4)) == s34n);
			REQUIRE(File::teststr(file4, File::read(file3)) == s43n);
		}
	}
}

//
// Created by hdaniel on 13/04/19.
//
#include <sstream>
#include <string>
#include <iostream>
#include <catch2/catch.hpp>
#include "evector.hpp"

#define DTYPE double


TEST_CASE( "Vector operations", "[evector]" ) {
	had::evector<DTYPE> v0 = {};
	had::evector<DTYPE> v1 = {1.1};
	had::evector<DTYPE> v2 = {1.1, 2.2};
	had::evector<DTYPE> v5 = {1.1, 2.2, 3.3, 4.4, 5.5};
	std::string v5str = "[ 1.1 2.2 3.3 4.4 5.5 ]";

    SECTION("Stream evector") {
		std::stringstream out;
        out << v5;
        REQUIRE(out.str() == v5str);
    }

    SECTION("Vector to string") {
		std::stringstream out;
        out << v5;
        REQUIRE(v5.toString() == v5str);
    }

    SECTION("Symmetric extension") {
		had::evector<DTYPE> vt1 = v5; //copies evector (needed since symmExt works in place)
		vt1.symmExt(3, 3);
		REQUIRE(vt1.toString() == "[ 3.3 2.2 1.1 1.1 2.2 3.3 4.4 5.5 5.5 4.4 3.3 ]");
		REQUIRE(vt1.toString('\n') == "[\n3.3\n2.2\n1.1\n1.1\n2.2\n3.3\n4.4\n5.5\n5.5\n4.4\n3.3\n]");

		had::evector<DTYPE> vt2 = v5; //copies evector (needed since symmExt works in place)
								 //needed new evector. reusing vt1 again will fail
		//int begin0 = WaveletTransform::extBeforeSize(4);
		//int end0 = WaveletTransform::extAfterSize(4, 1)
		int begin0 = 2;
		int end0 = 3;
        vt2.symmExt(begin0, end0); //odd
        REQUIRE(vt2.toString() == "[ 2.2 1.1 1.1 2.2 3.3 4.4 5.5 5.5 4.4 3.3 ]");

		had::evector<DTYPE> vt3 = v5; //copies evector (needed since symmExt works in place)
								 //needed new evector. reusing vt1 again will fail
		//int begin1 = WaveletTransform::extBeforeSize(4);
		//int end1 = WaveletTransform::extAfterSize(4, 1)
		int begin1 = 2;
		int end1 = 2;
		vt3.symmExt(begin1, end1); //even
		REQUIRE(vt3.toString() == "[ 2.2 1.1 1.1 2.2 3.3 4.4 5.5 5.5 4.4 ]");

		had::evector<DTYPE> vt4 = v0; //copies evector (needed since symmExt works in place)
								 //needed new evector. reusing vt1 again will fail
		REQUIRE_THROWS_AS(vt4.symmExt(1,1), std::length_error);

		had::evector<DTYPE> vt5 = v1; //copies evector (needed since symmExt works in place)
								 //needed new evector. reusing vt1 again will fail
		vt5.symmExt(2, 2);
		REQUIRE(vt5.toString() == "[ 1.1 1.1 1.1 1.1 1.1 ]");

		had::evector<DTYPE> vt6 = v2; //copies evector (needed since symmExt works in place)
								 //needed new evector. reusing vt1 again will fail
		vt6.symmExt(5, 5);
		REQUIRE(vt6.toString() == "[ 1.1 1.1 2.2 2.2 1.1 1.1 2.2 2.2 1.1 1.1 2.2 2.2 ]");
	}
}


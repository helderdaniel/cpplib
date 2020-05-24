/*
	StopWatch class simple test and demo
	v2.0 hdaniel@ualg.pt 2011
	v2.1 hdaniel@ualg.pt 2019 apr
	Changelog:
 		full c++ implementation
*/

#include <iostream>
#include "stopwatch.h"

using namespace std;

int main (int argc, char** argv) {
StopWatch sw;

	sw.reset();
	for (int i=0; i<1000000; i++)
		cout << i << endl;
	sw.lap();

	cout << sw << endl;	

	return 0;
}

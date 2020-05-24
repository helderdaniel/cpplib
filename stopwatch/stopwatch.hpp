/**
 * StopWatch class definition
 * v2.0 hdaniel@ualg.pt 2011
 * v2.1 hdaniel@ualg.pt 2019 apr
 * v3.0 hdaniel@ualg.pt 2019 apr C++14
 * v3.1 hdaniel@ualg.pt 2019 may C++14
 * Changelog:
 *      return time() from last reset() to last lap()
 *      Note:  Time is always real time, so it counts user inputs and event waits
 */

#ifndef __STOPWATCH_HPP__
#define __STOPWATCH_HPP__

#include <chrono>
#include <iostream>
using namespace std;
using namespace::std::chrono;


class StopWatch {
time_point<high_resolution_clock> start, stop;
double elapsedTime;  //real elapsed time in seconds

public:
	/**
	 * Creates StopWatch object and resets timer
	 */
	StopWatch() { reset(); }

	/**
	 * sets start and stop times equals to current time
	 */
	StopWatch& reset() {
		elapsedTime = 0;
		start = stop = high_resolution_clock::now();
		return *this; //for method chain
	}

	/**
	 * sets stop times equals to current time
	 */
	StopWatch& lap() {
		stop = high_resolution_clock::now();
		elapsedTime = duration<double>(stop - start).count();
		return *this; //for method chain
	}

	/**
	 * Returns time elapsed since last reset() until last lap()
	 * DOES count user input like wait keypress
	 */
	double watch() {
		return elapsedTime;
		//No method chain here, since returns time
	}

	//friend ostream& operator << (ostream& os, StopWatch& c);
};

/**
 * Prints real elapsed time in seconds
 */
ostream& operator << (ostream &os, StopWatch& sw) {
	os << sw.watch() << "s";
	return os;	
}

#endif //__STOPWATCH_HPP__


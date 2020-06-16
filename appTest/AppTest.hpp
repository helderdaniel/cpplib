/**
 * Unit test of apps utilities
 *
 * @author hdaniel@ualg.pt
 * @version 0.1
 * 16/06/2020.
 */


#ifndef __APPTEST_HPP__
#define __APPTEST_HPP__

#include <string>
#include <vector>
#include <iostream>

namespace had {

	class AppTest {

		std::stringstream simulatecin;
		std::stringstream simulatecout;

	public:
		typedef std::vector<const char *> cmdlineargs;

		template<typename Func>
		void exec(Func f, cmdlineargs cmdline) {
			f(cmdline.size(),
			  const_cast<char **>(cmdline.data()),
			  simulatecin,
			  simulatecout);
		}

		void clearINstream()  { simulatecin.str(""); }
		void clearOUTstream() { simulatecout.str(""); }

		void inStream(std::string in) { simulatecin.str(in); }
		std::string outStream() { return simulatecout.str(); }
	};
};

#endif //__APPTEST_HPP__

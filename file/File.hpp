/**
 * File utilities
 *
 * hdaniel@ualg.pt
 * 24/05/2020
 */

#ifndef MOOSHAKTOOLS_FILE_H
#define MOOSHAKTOOLS_FILE_H

#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <filesystem>
#include <regex>
#include <istream>
#include <fstream>

namespace fs = std::filesystem;

namespace had {

	class File {
		static const int bufsize = 4096;  //4k page size
		static const int maxLinesize = 256;

		/**
		 * Throws exception on file I/O error
		 *
		 * @param fname name of file
		 */
		static void fileError(const std::string &fname) {
			throw std::runtime_error(fname + " error: " + std::to_string(errno));
		}


		/**
		 *
		 * @param fn0 filename of a file to open
		  * @param fn1 filename of another file to open
		 * @param sf0 first opened file descriptor
		 * @param sf1 second opened file descriptor
		 */
		static void openFiles(const std::string &fn0, const std::string &fn1, FILE *&sf0, FILE *&sf1) {
			sf0 = fopen(fn0.c_str(), "r");
			if (!sf0) fileError(fn0);

			sf1 = fopen(fn1.c_str(), "r");
			if (!sf1) fileError(fn1);
		}


		/**
		 *
		 * @param sf0 first opened file descriptor
		 * @param sf1 second opened file descriptor
		 */
		static void closeFiles(FILE *const sf0, FILE *const sf1) {
			fclose(sf0);
			fclose(sf1);
		}


		/**
		 *
		 * @param sf0 first opened file descriptor
		 * @param sf1 second opened file descriptor
		 */
		static void rwdFiles(FILE *const sf0, FILE *const sf1) {
			fseek(sf0, 0L, SEEK_SET);
			fseek(sf1, 0L, SEEK_SET);
		}


		/**
		 *
		 * @param sf file to read
		 * @return the contents of sf as a string
		 */
		static std::string _read(FILE *const sf) {
			std::array<char, bufsize> buffer;
			int count;
			std::string ret;

			do {
				count = fread(buffer.data(), 1, bufsize, sf);
				ret.insert(ret.end(), std::begin(buffer), std::next(std::begin(buffer), count));
			} while (count > 0);

			return ret;
		}


		/**
		 * @param  sf0 a file descriptor
		   * @param  sf1 another file descriptor
		  * @return true if both file sizes are equal
		  */
		static bool _cmpsize(FILE *const sf0, FILE *const sf1) {

			//get fn0 size
			fseek(sf0, 0L, SEEK_END);
			long long sz0 = ftell(sf0);

			//get fn0 size
			fseek(sf1, 0L, SEEK_END);
			long long sz1 = ftell(sf1);

			//rewind files
			rwdFiles(sf0, sf1);

			return sz0 == sz1;
		}


		/**
		 * @param  sf0 a file descriptor
		   * @param  sf1 another file descriptor
		  * @return true if files sf0 and sf1 reference files with equal contents
		 */
		static bool _cmpbin(FILE *const sf0, FILE *const sf1) {
			std::array<char, bufsize> buf0;
			std::array<char, bufsize> buf1;
			bool ret = true;
			size_t count0, count1;

			//false if size differs
			if (!_cmpsize(sf0, sf1)) return false;

			//files are same size if reaches here
			do {
				count0 = fread(buf0.data(), 1, bufsize, sf0);
				count1 = fread(buf1.data(), 1, bufsize, sf1);

				//Something wrong when reading
				//since files have same size here
				if (count0 != count1) throw std::runtime_error("fread() error: " + std::to_string(errno));

				for (int i = 0; i < count0; i++)
					if (buf0[i] != buf1[i]) {
						ret = false;
						break;
					}

			} while (count0 > 0);

			return ret;
		}

		/**
		 * @param  sf0 a stream
		   * @param  sf1 is another stream to compare against
		  * @return true if streams sf0 and sf1 have equal contents
		 */
		static bool _cmpbin(std::istream sf0, std::istream sf1) {
			std::array<char, bufsize> buf0;
			std::array<char, bufsize> buf1;
			bool ret = true;

			std::streamsize count0, count1;

			//can not get length of all stream types
			//must always compare
			do {
				sf0.read(buf0.data(), bufsize);
				count0 = sf0.gcount();
				sf1.read(buf1.data(), bufsize);
				count1 = sf1.gcount();

				//streams have different length
				if (count0 != count1) {
					ret = false;
					break;
				}

				//compare stream contents
				for (int i = 0; i < count0; i++)
					if (buf0[i] != buf1[i]) {
						ret = false;
						break;
					}

			} while (count0 > 0);

			return ret;
		}


		/**
		 * @param expected 	a stream
		 * @param actual 	another stream to compare against
		 * @return if stream contents are equal returns empty string ""
		 * 		   if NOT returns a string with both streams' contents, preceded by tags:
		 * 				Expected:
		 * 				<fn0>
		 * 				Actual:
		 * 				<fn0>
		 */
		static std::string _test(std::istream &expected, std::istream &actual) {
			std::string sexp = File::read(expected);
			std::string sact = File::read(actual);
			std::string ret;

			//if strings differ
			if (sexp != sact)
				ret = "Expected:\n" + sexp + "Actual:\n" + sact;
			return ret;
		}


	public:
		/**
		 *
		 * @param fn filename of file to read
		 * @return the contents of fn as a string
		 */
		static std::string read(const std::string &fn) {
			FILE *const sf = fopen(fn.c_str(), "r");
			if (!sf) fileError(fn);
			std::string ret = _read(sf);
			fclose(sf);
			return ret;
		}


		/**
		 *
		 * @param sf stream to read
		 * @return the contents of sf as a string
		 */
		static std::string read(std::istream &sf) {
			return {std::istreambuf_iterator<char>(sf), std::istreambuf_iterator<char>()};
			//low level way
			/*
			std::array<char, bufsize> buffer;
			std::streamsize count;
			std::string ret;
			do {
				sf.read(buffer.data(), bufsize);
				count = sf.gcount();
				ret.insert(ret.end(), std::begin(buffer), std::next(std::begin(buffer), count));
			} while (count>0);

			return ret;
			*/

		}


		/**
		 * @param  fn0 filename of a file
		 * @param  fn1 filename of another file to compare against
		 * @return true if both file sizes are equal
		 */
		static bool cmpsize(const std::string &fn0, const std::string &fn1) {
			FILE *sf0, *sf1;

			openFiles(fn0, fn1, sf0, sf1);
			bool ret = _cmpsize(sf0, sf1);
			closeFiles(sf0, sf1);
			return ret;
		}


		/**
		 * @param  fn0 filename of a file to compare
		 * @param  fn1 filename of file to compare against
		 * @return true if files fn0 and fn1 are equal
		 */
		static bool cmpbin(const std::string &fn0, const std::string &fn1) {
			FILE *sf0, *sf1;

			openFiles(fn0, fn1, sf0, sf1);
			bool ret = _cmpbin(sf0, sf1);
			closeFiles(sf0, sf1);
			return ret;
		}


		/**
		 *
		 * @param fn0 filename of a file to compare
		 * @param fn1 filename of file to compare against
		 * @return string with differences in both files:
		 * 				<line>:
		 * 				<fn0 line>
		 * 				<fn1 line>
		 */
		static std::string cmptext(const std::string &fn0, const std::string &fn1) {
			std::array<char, maxLinesize> buf0;
			std::array<char, maxLinesize> buf1;
			std::string ret;
			int line = 0;
			char *str0, *str1;
			FILE *sf0, *sf1;

			openFiles(fn0, fn1, sf0, sf1);
			for (;;) {
				str0 = fgets(buf0.data(), maxLinesize, sf0);
				str1 = fgets(buf1.data(), maxLinesize, sf1);

				//both NULL end (empty files are equal)
				if (!str0 && !str1) break;

				//default false (if one is null)
				int equal = 0;

				//General case (both NOT null)
				if (str0 && str1) equal = !strcmp(str0, str1);

				//if different
				if (!equal) {
					char head[10];
					sprintf(head, "%d:\n", line);
					ret += head;
					if (str0 != NULL) ret += buf0.data();
					if (str1 != NULL) ret += buf1.data();
				}

				++line;
			}

			closeFiles(sf0, sf1);
			return ret;
		}


		/*
		static std::string test(const std::string& expected, const std::string& actual) {
			std::string ret;
			FILE *sfexp, *sfact;

			openFiles(expected, actual, sfexp, sfact);

			//if size differs return both files
			if (!_cmpbin(sfexp, sfact)) {
				//rewind files
				rwdFiles(sfexp, sfact);

				ret += "Expected:\n";
				ret += _read(sfexp);
				ret += "Actual:\n";
				ret += _read(sfact);
			}

			closeFiles(sfexp, sfact);
			return ret;
		}
		*/

		/**
		 * @param expected filename of a file to compare
		 * @param actual   filename of file to compare against
		 * @return if files equal returns empty string ""
		 * 		   if NOT returns a string with both files preceded by tags:
		 * 				Expected:
		 * 				<fn0>
		 * 				Actual:
		 * 				<fn0>
		 */
		static std::string test(const std::string &expected, const std::string &actual) {
			std::ifstream exp(expected, std::ifstream::binary);
			std::ifstream act(actual, std::ofstream::binary);
			return _test(exp, act);
		}


		/**
		 * @param expected 	a filename
		 * @param actual 	a string
		 * @return if contents of filename are equal to string actual returns empty string ""
		 * 		   if NOT returns a string with file and string contents, preceded by tags:
		 * 				Expected:
		 * 				<fn0>
		 * 				Actual:
		 * 				<fn0>
		 */
		static std::string teststr(const std::string &expected, const std::string &actual) {
			std::ifstream exp;
			exp.exceptions(std::ifstream::failbit);
			try {
				exp.open(expected, std::ifstream::binary);
			} catch (const std::exception &e) {
				fileError(expected);
			}
			std::stringstream act(actual);
			return _test(exp, act);
		}


		/**
		 *
		 * @param dir    root dir to search
		 * @param regex  filename regex
		 * @param depth  maximum recursion depth (root dir has depth 0)
		 * @return vector of paths that matches regex, form root directory dir
		 */
		static std::vector<fs::path> search(const std::string &dir, const std::string &regex, const int depth = -1) {
			std::vector<fs::path> v;

			auto it = fs::recursive_directory_iterator(dir);
			for (auto &p: it) {
				int id = it.depth();
				if (depth >= 0 && id > depth) continue;
				if (std::regex_match(p.path().filename().c_str(), std::regex(regex)))
					v.push_back(p.path());
			}
			return v;
		}

	};

}

#endif //MOOSHAKTOOLS_FILE_H

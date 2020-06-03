/**
 * File utilities
 *
 * hdaniel@ualg.pt
 * 24/05/2020
 */

#ifndef MOOSHAKTOOLS_FILE_H
#define MOOSHAKTOOLS_FILE_H

#include <fstream>
#include <istream>
#include <regex>
#include <filesystem>

using std::string;
using std::vector;
using std::array;
using std::exception;
using std::runtime_error;
using std::to_string;
using std::streamsize;
using std::istream;
using std::ifstream;
using std::istreambuf_iterator;
using std::next;
using std::ostream;
using std::ofstream;
using std::stringstream;
using std::regex;

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
		static void fileError(const string &fname, const int err=errno) {
			throw runtime_error(fname + " error: " + to_string(err));
		}


		/**
		 *
		 * @param fn0 filename of a file to open
		 * @param fn1 filename of another file to open
		 * @param sf0 first opened file descriptor
		 * @param sf1 second opened file descriptor
		 * @param mode0 file fn0 open mode, default "r", to write specify "w", to read and write "r+"
		 * @param mode1 file fn0 open mode, default "r", to write specify "w", to read and write "r+"
		 */
		static void openFiles(const string &fn0, const string &fn1, FILE *&sf0, FILE *&sf1,
							  const string &mode0="r", const string &mode1="r") {
			sf0 = fopen(fn0.c_str(), mode0.c_str());
			if (!sf0) fileError(fn0);

			sf1 = fopen(fn1.c_str(), mode1.c_str());
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
		static string _read(FILE *const sf) {
			array<char, bufsize> buffer;
			int count;
			string ret;

			do {
				count = fread(buffer.data(), 1, bufsize, sf);
				ret.insert(ret.end(), begin(buffer), next(begin(buffer), count));
			} while (count > 0);

			return ret;
		}


		/**
		 * @param sf file to be overwritten
		 * @param str string to be written
		 */
		static void _write(FILE *const sf, const string &str) {
			int count, idx=0;

			fwrite(&str[idx], 1, str.size(), sf);
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
			array<char, bufsize> buf0;
			array<char, bufsize> buf1;
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
				if (count0 != count1) throw runtime_error("fread() error: " + to_string(errno));

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
		static bool _cmpbin(istream sf0, istream sf1) {
			array<char, bufsize> buf0;
			array<char, bufsize> buf1;
			bool ret = true;

			streamsize count0, count1;

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
		static string _test(istream &expected, istream &actual) {
			string sexp = File::read(expected);
			string sact = File::read(actual);
			string ret;

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
		static string read(const string &fn) {
			FILE *const sf = fopen(fn.c_str(), "r");
			if (!sf) fileError(fn);
			string ret = _read(sf);
			fclose(sf);
			return ret;
		}


		/**
		 *
		 * @param fn filename of file to be overwritten
		 * @param str string to be written
		 */
		static void write(const string &fn, const string &str) {
			FILE *const sf = fopen(fn.c_str(), "w");
			if (!sf) fileError(fn);
			_write(sf, str);
			fclose(sf);
		}


		/**
		 *
		 * @param sf stream to read
		 * @return the contents of sf as a string
		 */
		static string read(istream &sf) {
			return {istreambuf_iterator<char>(sf), istreambuf_iterator<char>()};
			//low level way
			/*
			array<char, bufsize> buffer;
			streamsize count;
			string ret;
			do {
				sf.read(buffer.data(), bufsize);
				count = sf.gcount();
				ret.insert(ret.end(), begin(buffer), next(begin(buffer), count));
			} while (count>0);

			return ret;
			*/

		}


		/**
		 * @param  fn0 filename of a file
		 * @param  fn1 filename of another file to compare against
		 * @return true if both file sizes are equal
		 */
		static bool cmpsize(const string &fn0, const string &fn1) {
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
		static bool cmpbin(const string &fn0, const string &fn1) {
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
		static string cmptext(const string &fn0, const string &fn1) {
			array<char, maxLinesize> buf0;
			array<char, maxLinesize> buf1;
			string ret;
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
		static string test(const string& expected, const string& actual) {
			string ret;
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
		static string test(const string &expected, const string &actual) {
			ifstream exp(expected, ifstream::binary);
			ifstream act(actual, ofstream::binary);
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
		static string teststr(const string &expected, const string &actual) {
			ifstream exp;
			exp.exceptions(ifstream::failbit);
			try {
				exp.open(expected, ifstream::binary);
			} catch (const exception &e) {
				fileError(expected);
			}
			stringstream act(actual);
			return _test(exp, act);
		}


		/**
		 *
		 * @param dir    root dir to search
		 * @param regex  filename regex
		 * @param depth  maximum recursion depth (root dir has depth 0)
		 * @return vector of paths that matches regex, form root directory dir
		 */
		static vector<fs::path> search(const string &dir, const string &regexstr, const int depth = -1) {
			vector<fs::path> v;

			auto it = fs::recursive_directory_iterator(dir);
			for (auto &p: it) {
				int id = it.depth();
				if (depth >= 0 && id > depth) continue;
				if (regex_match(p.path().filename().c_str(), regex(regexstr)))
					v.push_back(p.path());
			}
			return v;
		}


		/**
		* Copies file src to file dest.
		* overwrites desd.
		*
		* @param fn0 filename of source file
		* @param fn1 filename of destination file
		*/
		static void copy(const string &src, const string &dst) {
			array<char, bufsize> buffer;
			FILE *sfsrc, *sfdst;
			int count;

			openFiles(src, dst, sfsrc, sfdst, "r", "w");
			clearerr(sfsrc);
			clearerr(sfdst);
			do {
				count = fread(buffer.data(), 1, bufsize, sfsrc);
				if (ferror(sfsrc) != 0)  fileError(src, -1);
				fwrite(buffer.data(), 1, count, sfdst);
				if (ferror(sfdst) != 0)  fileError(src, -1);

			} while (count > 0);

			closeFiles(sfsrc, sfdst);
		}

	};

}

#endif //MOOSHAKTOOLS_FILE_H

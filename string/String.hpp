/**
 *
 * @author hdaniel@ualg.pt 
 * @version 0.1
 * 27/05/2020.
 */

#ifndef __HAD_STRING_HPP
#define __HAD_STRING_HPP

#include <cstring>
#include <regex>

using std::string;
using std::regex;
using std::regex_search;
using std::sregex_iterator;

namespace had {

	class String {

		static inline const char* alphaNum =
				"0123456789"
				"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
				"abcdefghijklmnopqrstuvwxyz";

	public:

		/**
		 * @param str    string to search
		 * @param bdelim before str delimiter
		 * @param edelim after str delimiter
		 * @param output first string delimited by bdelim<string>edelim or "" if not found
		 * @param pos    first character to search
		 *
		 * @pre		<= 0 pos < str.length()
		 *
		 * @return position of character after edelim or -1 if not found
		 */
		static ulong firstSubstring(const string &str, const string &bdelim,
									const string &edelim, string &output,
									const long pos = 0) {
			output = "";
			int begin = str.find(bdelim, pos);
			if (begin == string::npos) return -1;
			begin += bdelim.length();

			int end = str.find(edelim, begin + 1);
			if (end == string::npos) return -1;

			output = str.substr(begin, end - begin);
			return end + edelim.length();
		}

		/**
		 * @param  str     string to search
		 * @param  bdelim  before str delimiter
		 * @param  edelim  after str delimiter
		 * @output param   last string delimited by bdelim<string>edelim or "" if not found
		 * @param  pos     last character to search
		 *
		 * @pre		<= 0 pos < str.length()
		 *
		 * @return position of character before bdelim or -1 if not found
		 */
		static ulong lastSubstring(const string &str, const string &bdelim,
								   const string &edelim, string &output,
								   const long pos = string::npos) {
			output = "";
			int end = str.find_last_of(edelim, pos);
			if (end == string::npos) return -1;
			end -= edelim.length();

			int begin = str.find_last_of(bdelim, end) + 1;
			if (begin == string::npos) return -1;
			output = str.substr(begin, end - begin + 1);
			return begin - bdelim.length() - 1;
		}

		/**
		 * @param  len length of string
		 * @return string with random alphanumeric [0-9] | [A-Z] | [a-z] characters
		 */
		static string randAlphaNum(const int len) {
			string ret;

			for (int i = 0; i < len; ++i)
				ret += alphaNum[std::rand() % strlen(alphaNum)];
			return ret;
		}

		/**
		 * @param  len length of string
		 * @return string with random characters
		 */
		static string rand(const int len) {
			string ret;

			for (int i = 0; i < len; ++i)
				ret += std::rand() % 256;
			return ret;
		}


		/**
		 * Replaces any str substrings that matches regex rgx.
		 * Works in place.
		 *
		 * @param str 			string to be operated
		 * @param rgx  			regex to search
		 * @param replacement 	replacement string
		 * @return true if at least one replacement took place
		 */
		static bool regex_replace(string &str, const string &rgx, const string& replacement) {
			regex regexp(rgx);

			//Search regex
			auto begin = sregex_iterator(str.begin(), str.end(), regexp);
			auto end = sregex_iterator();

			//replace using iterator
			for (auto i = begin; i != end; ++i)
				str.replace(i->position(), i->length(), replacement);

			//returns true if at least one match was found and replaced
			return (begin != end);
		}
	};

}

#endif //__HAD_STRING_HPP

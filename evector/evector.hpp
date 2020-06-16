//
// Created by hdaniel on 20/04/19.
//
// v1.0
//

#ifndef __HAD_EVECTOR_HPP__
#define __HAD_EVECTOR_HPP__


/*
 * JLBorges
 * http://www.cplusplus.com/forum/general/156923/
 *
 * taken from:
 *
 * Stroustrup in A Tour of C++ (draft)
 * http://isocpp.org/files/papers/4-Tour-Algo-draft.pdf
 * pag. 104
 *
 * It is ok to inherit publicly from std::vector<> as long as:

a. We remember that every operation provided by std::vector<> must be a semantically valid
   operation on an object of the derived class

b. We avoid creating derived class objects with dynamic storage duration.
   (std::vector<> does not have a virtual destructor).

The standard library vector does not guarantee range checking. For example:

vector<Entry> phone_book(1000);
int i = phone_book[2001].number; // 2001 is out of range

That initialization is likely to place some random value in i rather than giving an error.
This is undesirable and out-of-range errors are a common problem.
Consequently, I often use a simple range-checking adaptation of vector:

template<typename T>
class Vec : public std::vector<T> {
public:
    using vector<T>::vector; // use the constructors from vector

    T& operator[](int i) { return vector<T>::at(i); } // range-checked

    const T& operator[](int i) const { return vector<T>::at(i); } // range-checked
    // for const objects;
};

Vec inherits everything from vector except for the subscript operations that it redefines to do range checking. The at() operation is a vector subscript operation that throws an exception of type out_of_range if its argument is out of the vector’s range.

An out-of-range access will throw an exception that the user can catch.
*/

#include <iomanip>
#include <vector>
#include <cmath>  //get right version of std::abs for any type
 				  //if not will use cstdlib abs which is fr integers only

using std::cout;
using std::fixed;
using std::setprecision;
using std::string;
using std::vector;
using std::ostream;
using std::stringstream;

namespace had {

	template<typename T>
	class evector : public vector<T> {
		//on to_string() consider zero if lower than printAsZero
		//this way avoids printing negative zeros: -0.000
		static constexpr double printAsZero = 1e-10;
		static constexpr char defaultSeparator = ' ';
		static constexpr int defaultPrecision = -1;
		static constexpr int defaultFixedPrecision = -1;

	public:
		//NOTE: does NOT convert from vector to evector
		using vector<T>::vector; //use the constructors from vector

		/**
		 * Extend vector, symmetric extension
		 *
		 * @param eb: number of elements to add at the beginning
		 * @param ea: number of elements to add at the end
		 *
		 * PRE: this.size() > 0
		 * Example: { 1, 2 }.symmExt(3,3) = { 2, 2, 1, 1, 2, 2, 1, 1}
		 *
		 */
		void symmExt(int eb, int ea) {
			int size = this->size();
			if (size <= 0)
				throw std::length_error("Can only extend vectors with size() > 0");


			//Symmetric extend
			//check if signal is large enough to symmetric extend.
			//If NOT extend until its possible and iterate on new partial
			//extended signal until full extension complete
			//TODO: maybe these loops can be simpler
			int ebfixed = eb;
			do {
				size = this->size();
				if (size - eb <= 0) {
					ebfixed = size;
					eb -= size;
				}
				else {
					ebfixed = eb;
					eb = 0;
				}
				this->insert(this->begin(), this->rend() - ebfixed, this->rend());
			} while (eb > 0);

			int eafixed = ea;
			do {
				size = this->size();
				if (size - ea <= 0) {
					eafixed = size;
					ea -= size;
				}
				else {
					eafixed = ea;
					ea = 0;
				}
				this->insert(this->end(), this->rbegin(), this->rbegin() + eafixed);
			} while (ea > 0);

			/*
			//implementation of: http://wavelet2d.sourceforge.net/ symm_ext()
			//does not extend signals smaller than the number of elements to extend
			unsigned int len = this->size();
			//int l = ??; //number of elements to add at front and back of this vector
						  //assumes: l = ea = eb
			for (int i =0; i < l; i++) {
				//rewrite with out cycle
				T temp1 = (*this)[i * 2];
				T temp2 = (*this)[len - 1];
				this->insert(this->begin(), temp1);
				this->insert(this->end(), temp2);
			}
			*/
		}

		/**
		 * @return average of vector
		 */
		double avg() {
			return accumulate(this->begin(), this->end(), 0.0) / this->size();
		}


		/**
		 * PRE: a.size() == b.size()
		 * POS: return have a.size()
		 *
		 * @param vector subtraction
		 * @return
		 */
		/*
		 * Buggy
		evector<T>& operator-=(const evector<T>&a) {
		   for (int i=0; i<a.size(); ++i)
			   *this[i]-=a[i];
		   return *this;
		}

		friend evector<T> operator-(evector<T>&a, const evector<T>& b) {
		   return a-=b;
		}

		inline bool operator<(const T&a){
		   for (int i=0; i<a.size(); ++i)
			   if (!(*this[i]<a[i])) return false;
		   return true;
		}

		friend inline bool operator<(const evector<T>&a, const evector<T>& b){
		   for (int i=0; i<a.size(); ++i)
			   if (!(a[i]<b[i])) return false;
		   return true;
		}
		*/

		/**
		*
		* @param sep separator character, default is a ASCII space
		* @param prec precision
		* @param fixedPrec fixed precision
		* @return vector as a string
		*/
		friend string to_string(const evector<T>& v,
						 const char sep = defaultSeparator,
						 const int prec = defaultPrecision,
						 const int fixedPrec = defaultFixedPrecision) {
			stringstream os;
			//override default precisions
			if (prec >= 0) os << setprecision(prec);
			if (fixedPrec >= 0) os << fixed << setprecision(fixedPrec);

			os << "[" << sep;
			for (int i = 0; i < v.size(); ++i) {
				double val = v[i];
				//Avoid printing negative zero: -0.0 for very small number near zero

				//use fabs(), cause cstdlib abs() will zero if -1 < val < 1 since is for integer
				//if (fabs(val) < printAsZero)

				//or include <cmath> and use std::abs() or std::fabs() as below:
				if (std::fabs(val) < printAsZero)
					val = 0.0;
				os << val << sep;
			}
			os << "]";
			return os.str();
		}

		//Could use Named Parameter Idiom
		//https://isocpp.org/wiki/faq/ctors#named-parameter-idiom
		//see also project namedParIdiom on these folders
		friend string to_string(const evector<T>& v, const int prec) { return to_string(v, defaultSeparator, prec); }
		//Cannot do the next overload, cause signature is equal to previous overloaded func: toString(int)
		//string toString(int fixedPrec) { return toString(defaultSeparator, defaultPrecision, fixedPrec); }
	};


	template<typename T>
	ostream &operator<<(ostream &os, const evector<T> &v) {
		os << to_string(v);
		return os;
	}

} //end namespace had

#endif //__HAD_EVECTOR_HPP__



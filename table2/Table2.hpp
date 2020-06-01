/**
 * Shell utilities
 * hdaniel@ualg.pt may 2020
 *
 * Associative array (map) that can be indexed by key and by value
 * Table is immutable, it must be fully created with constructor
 *
 * Uses a dual representation of the table to be accessed by value
 */

#ifndef MOOSHAKTOOLS_TABLE2_H
#define MOOSHAKTOOLS_TABLE2_H

#include <map>

using std::map;

namespace had {

	template <class K, class V>
	class Table2 {

		map<K,V> table;
		map<V,K> tablerev;
		//map<const V*,const K*> tablerev;

	public:

		/**
		 * @param table map<K,V> that represents the table Keys and Values
		 * @pre   m must not have duplicated values
		 */
		 //using std::multimap would allow duplicate keys and values to be reverted
		 //
		 /*
		 //maps are constant so there is no way of deleting
		 //from table map and leave pointers to nowhere in tablerev
		 //Also parameter map is copied
		 //Note: c++ does not accept maps of references
		*/

		Table2(const map<K,V> &m) : table(m) {
			//create reverse table of pointers
			typename map<K, V>::iterator it;
			for (it = table.begin(); it != table.end(); ++it) {
				tablerev[it->second] = it->first;
				//const K *k = &(it->first);
				//const V *v = &(it->second);
				//tablerev[v] = k;
			}
		}

		/**
		 * @return number of key, value pairs in table
		 */
		int size() const { return table.size(); }

		/**
		 * @param key to search value
		 * @return value at key
		 */
		const V& value(K key) const { return table.at(key); }

		/**
		 * @param value to search key
		 * @return key at value
		 */
		const K& key(V value) const { return tablerev.at(value); }
	};

}

#endif //MOOSHAKTOOLS_TABLE2_H

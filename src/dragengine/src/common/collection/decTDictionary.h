/*
 * MIT License
 *
 * Copyright (C) 2025, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DECTDICTIONARY_H_
#define _DECTDICTIONARY_H_

#include "../string/decString.h"
#include "../string/decStringList.h"
#include "../exceptions.h"
#include "decTList.h"


/**
 * \brief Dictionary template class mapping values to keys.
 * 
 * For type K it is required to exist a global function "unsigned int DEHash(const K&)".
 * Such functions are provided in decHashFunctions.h (int, unsigned int, void*) as well as
 * certain classes like decString.
 */
template<typename K, typename V, typename VP = V>
class decTDictionary{
private:
	struct sDictEntry{
		unsigned int hash;
		K key;
		V value;
		sDictEntry *next;
		
		sDictEntry() : hash(0), next(nullptr){
		}
		
		sDictEntry(const sDictEntry &entry) :
		hash(entry.hash),
		key(entry.key),
		value(entry.value),
		next(nullptr){
		}
		
		sDictEntry(unsigned int nhash, const K &nkey, const VP &nvalue) :
		hash(nhash),
		key(nkey),
		value(nvalue),
		next(nullptr){
		}
		
		~sDictEntry(){
		}
		
		void SetValue(const VP &nvalue){
			value = nvalue;
		}
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new dictionary. */
	decTDictionary() : pBuckets(nullptr), pBucketCount(8), pEntryCount(0){
		pBuckets = new sDictEntry*[pBucketCount];
		int i;
		for(i=0; i<pBucketCount; i++){
			pBuckets[i] = nullptr;
		}
	}
	
	/**
	 * \brief Create a new dictionary with initial bucket count.
	 * \throws deeInvalidParam \em bucketCount is less than 1.
	 */
	explicit decTDictionary(int bucketCount) : pBuckets(nullptr), pBucketCount(bucketCount), pEntryCount(0){
		DEASSERT_TRUE(bucketCount >= 1)
		
		pBuckets = new sDictEntry*[bucketCount];
		int i;
		for(i=0; i<bucketCount; i++){
			pBuckets[i] = nullptr;
		}
	}
	
	/** \brief Create copy of a dictionary. */
	decTDictionary(const decTDictionary<K,V,VP> &dict) : pBuckets(nullptr), pBucketCount(dict.pBucketCount), pEntryCount(0){
		pBuckets = new sDictEntry*[pBucketCount];
		int i;
		for(i=0; i<pBucketCount; i++){
			pBuckets[i] = nullptr;
		}
		
		for(i=0; i<dict.pBucketCount; i++){
			sDictEntry *iterEntry = dict.pBuckets[i];
			while(iterEntry){
				SetAt(iterEntry->key, iterEntry->value);
				iterEntry = iterEntry->next;
			}
		}
	}
	
	/** \brief Clean up the dictionary. */
	~decTDictionary(){
		RemoveAll();
		if(pBuckets){
			delete [] pBuckets;
		}
	}
	/*@}*/
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/** \brief Dictionary is empty. */
	inline bool IsEmpty() const{ return pEntryCount == 0; }
	
	/** \brief Dictionary is not empty. */
	inline bool IsNotEmpty() const{ return pEntryCount > 0; }
	
	/**
	 * \brief Determine if a key is located in the dictionary.
	 */
	bool Has(const K &key) const{
		return pGetEntry(key) != nullptr;
	}
	
	/**
	 * \brief Value for key.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	const V &GetAt(const K &key) const{
		const sDictEntry * const entry = pGetEntry(key);
		DEASSERT_NOTNULL(entry)
		
		return entry->value;
	}
	
	/**
	 * \brief Value for key.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	V &GetAt(const K &key){
		sDictEntry * const entry = pGetEntry(key);
		DEASSERT_NOTNULL(entry)
		
		return entry->value;
	}
	
	/**
	 * \brief Value by key
	 * \retval true Value of \em key stored in \em value.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt(const K &key, V *value) const{
		const sDictEntry * const entry = pGetEntry(key);
		if(entry){
			if(value){
				*value = entry->value;
			}
			return true;
		}
		return false;
	}
	
	/**
	 * \brief Value for key or default value if absent.
	 */
	V GetAtOrDefault(const K &key, const V &defaultValue) const{
		const sDictEntry * const entry = pGetEntry(key);
		return entry ? entry->value : defaultValue;
	}
	
	/**
	 * \brief Set key to value.
	 */
	void SetAt(const K &key, const VP &value){
		sDictEntry *entry = pGetEntry(key);
		
		if(entry){
			entry->SetValue(value);
			
		}else{
			const unsigned int hash = DEHash(key);
			const int bucketIndex = hash % pBucketCount;
			
			entry = new sDictEntry(hash, key, value);
			entry->next = pBuckets[bucketIndex];
			pBuckets[bucketIndex] = entry;
			pEntryCount++;
		}
		
		CheckLoad();
	}
	
	/**
	 * \brief Remove a key.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void Remove(const K &key){
		DEASSERT_TRUE(RemoveIfPresent(key))
	}
	
	/**
	 * \brief Remove a key if present in the dictionary.
	 * \returns true if removed.
	 */
	bool RemoveIfPresent(const K &key){
		const unsigned int hash = DEHash(key);
		const int bucketIndex = hash % pBucketCount;
		sDictEntry *iterEntry = pBuckets[bucketIndex];
		sDictEntry *lastEntry = nullptr;
		
		while(iterEntry){
			if(iterEntry->hash == hash && iterEntry->key == key){
				if(lastEntry){
					lastEntry->next = iterEntry->next;
					
				}else{
					pBuckets[bucketIndex] = iterEntry->next;
				}
				
				pEntryCount--;
				delete iterEntry;
				return true;
			}
			
			lastEntry = iterEntry;
			iterEntry = iterEntry->next;
		}
		return false;
	}
	
	/** \brief Remove all keys from the dictionary. */
	void RemoveAll(){
		if(pBuckets){
			int i;
			for(i=0; i<pBucketCount; i++){
				if(pBuckets[i]){
					sDictEntry *iterEntry = pBuckets[i];
					pBuckets[i] = nullptr;
					
					while(iterEntry){
						sDictEntry * const delbucket = iterEntry;
						iterEntry = iterEntry->next;
						delete delbucket;
					}
				}
			}
		}
		
		pEntryCount = 0;
	}
	
	/** \brief List of all keys in the dictionary. */
	decTList<K> GetKeys() const{
		decTList<K> keys(pEntryCount);
		int i;
		
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = pBuckets[i];
			
			while(iterEntry){
				keys.Add(iterEntry->key);
				iterEntry = iterEntry->next;
			}
		}
		
		return keys;
	}
	
	/** \brief List with all values in the dictionary. */
	decTList<V> GetValues() const{
		decTList<V> values(pEntryCount);
		int i;
		
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = pBuckets[i];
			
			while(iterEntry){
				values.Add(iterEntry->value);
				iterEntry = iterEntry->next;
			}
		}
		
		return values;
	}
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals(const decTDictionary<K,V,VP> &dict) const{
		if(dict.pEntryCount != pEntryCount){
			return false;
		}
		
		int i;
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = pBuckets[i];
			
			while(iterEntry){
				sDictEntry * const otherEntry = dict.pGetEntry(iterEntry->key);
				if(!otherEntry || !(otherEntry->value == iterEntry->value)){
					return false;
				}
				iterEntry = iterEntry->next;
			}
		}
		
		return true;
	}
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad(){
		if((float)pEntryCount / (float)pBucketCount < 0.7f){
			return;
		}
		
		const int newBucketCount = pBucketCount + (pBucketCount >> 1); // +50%
		sDictEntry ** const newBuckets = new sDictEntry*[newBucketCount];
		int i;
		
		for(i=0; i<newBucketCount; i++){
			newBuckets[i] = nullptr;
		}
		
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = pBuckets[i];
			
			while(iterEntry){
				sDictEntry * const moveEntry = iterEntry;
				iterEntry = iterEntry->next;
				
				const int bucketIndex = moveEntry->hash % newBucketCount;
				moveEntry->next = newBuckets[bucketIndex];
				newBuckets[bucketIndex] = moveEntry;
			}
		}
		
		delete [] pBuckets;
		pBuckets = newBuckets;
		pBucketCount = newBucketCount;
	}
	
	
	/**
	 * \brief Visit elements.
	 * \param[in] visitor Visitor callable invoked as visitor(K,V).
	 */
	template<typename Visitor>
	void Visit(Visitor &visitor) const {
		int i;
		for(i=0; i<pBucketCount; i++){
			const sDictEntry *iterEntry = pBuckets[i];
			while(iterEntry){
				visitor(iterEntry->key, iterEntry->value);
				iterEntry = iterEntry->next;
			}
		}
	}
	
	template<typename Visitor>
	void Visit(Visitor &&visitor) const{
		Visit<Visitor>(visitor);
	}
	
	
	/**
	 * \brief Find value.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(K,V).
	 * \param[out] found Found value if true is returned.
	 */
	template<typename Evaluator>
	bool Find(Evaluator &evaluator, const V* &found) const{
		int i;
		for(i=0; i<pBucketCount; i++){
			const sDictEntry *iterEntry = pBuckets[i];
			while(iterEntry){
				if(evaluator(iterEntry->key, iterEntry->value)){
					found = &iterEntry->value;
					return true;
				}
				iterEntry = iterEntry->next;
			}
		}
		
		found = nullptr;
		return false;
	}
	
	template<typename Evaluator>
	bool Find(Evaluator &&evaluator, const V* &found) const{
		return Find<Evaluator>(evaluator, found);
	}
	
	
	/**
	 * \brief Find value with default value.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(K,V).
	 * \return Found value or default value if not found.
	 */
	template<typename Evaluator>
	V FindDefault(Evaluator &evaluator, const V &defaultValue) const{
		const V *found = nullptr;
		return Find<Evaluator>(evaluator, found) ? *found : defaultValue;
	}
	
	template<typename Evaluator>
	V FindDefault(Evaluator &&evaluator, const V &defaultValue) const{
		return FindDefault<Evaluator>(evaluator, defaultValue);
	}
	
	
	/**
	 * \brief Collect elements into a new dictionary.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(K,V).
	 */
	template<typename Evaluator>
	decTDictionary<K,V,VP> Collect(Evaluator &evaluator) const{
		decTDictionary<K,V,VP> collected;
		int i;
		
		for(i=0; i<pBucketCount; i++){
			const sDictEntry *iterEntry = pBuckets[i];
			
			while(iterEntry){
				if(evaluator(iterEntry->key, iterEntry->value)){
					collected.SetAt(iterEntry->key, iterEntry->value);
				}
				iterEntry = iterEntry->next;
			}
		}
		
		return collected;
	}
	
	template<typename Evaluator>
	decTDictionary<K,V,VP> Collect(Evaluator &&evaluator) const{
		return Collect<Evaluator>(evaluator);
	}
	
	
	/**
	 * \brief Remove elements matching condition.
	 * \param[in] evaluator Evaluator callable invoked as evaluator(K,V).
	 */
	template<typename Evaluator>
	void RemoveIf(Evaluator &evaluator){
		int i;
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = pBuckets[i];
			sDictEntry *lastEntry = nullptr;
			
			while(iterEntry){
				if(evaluator(iterEntry->key, iterEntry->value)){
					sDictEntry * const delEntry = iterEntry;
					
					if(lastEntry){
						lastEntry->next = iterEntry->next;
						
					}else{
						pBuckets[i] = iterEntry->next;
					}
					
					iterEntry = iterEntry->next;
					delete delEntry;
					pEntryCount--;
					
				}else{
					lastEntry = iterEntry;
					iterEntry = iterEntry->next;
				}
			}
		}
	}
	
	template<typename Evaluator>
	void RemoveIf(Evaluator &&evaluator){
		RemoveIf<Evaluator>(evaluator);
	}
	/*@}*/
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==(const decTDictionary<K,V,VP> &dict) const{
		return Equals(dict);
	}
	
	/** \brief Determine if dictionary is not equal to another dictionary. */
	bool operator!=(const decTDictionary<K,V,VP> &dict) const{
		return !Equals(dict);
	}
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decTDictionary<K,V,VP> operator+(const decTDictionary<K,V,VP> &dict) const{
		decTDictionary<K,V,VP> ndict(*this);
		int i;
		
		for(i=0; i<dict.pBucketCount; i++){
			sDictEntry *iterEntry = dict.pBuckets[i];
			
			while(iterEntry){
				ndict.SetAt(iterEntry->key, iterEntry->value);
				iterEntry = iterEntry->next;
			}
		}
		
		return ndict;
	}
	
	/**
	 * \brief Value for key.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	const V &operator[](const K &key) const{
		return GetAt(key);
	}
	
	/**
	 * \brief Value for key.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	V &operator[](const K &key){
		return GetAt(key);
	}
	
	/** \brief Copy dictionary to this dictionary. */
	decTDictionary<K,V,VP> &operator=(const decTDictionary<K,V,VP> &dict){
		if(&dict == this){
			return *this;
		}
		
		RemoveAll();
		
		int i;
		for(i=0; i<dict.pBucketCount; i++){
			sDictEntry *iterEntry = dict.pBuckets[i];
			
			while(iterEntry){
				SetAt(iterEntry->key, iterEntry->value);
				iterEntry = iterEntry->next;
			}
		}
		
		return *this;
	}
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decTDictionary<K,V,VP> &operator+=(const decTDictionary<K,V,VP> &dict){
		int i;
		
		for(i=0; i<dict.pBucketCount; i++){
			sDictEntry *iterEntry = dict.pBuckets[i];
			
			while(iterEntry){
				SetAt(iterEntry->key, iterEntry->value);
				iterEntry = iterEntry->next;
			}
		}
		
		return *this;
	}
	/*@}*/
	
	
	/** \name Standard library iterators. */
	/*@{*/
	class const_iterator{
	public:
		using value_type = std::pair<const K&, const V&>;
		using reference = value_type;
		using pointer = void;
		using difference_type = std::ptrdiff_t;

		const_iterator() : pDict(nullptr), bucketIndex(0), entry(nullptr) {}
		const_iterator(const decTDictionary *d, int b, const sDictEntry *e)
			: pDict(d), bucketIndex(b), entry(e)
		{
			if(pDict && bucketIndex < pDict->pBucketCount && entry == nullptr){
				entry = pDict->pBuckets[bucketIndex];
			}
			advance_to_valid();
		}

		bool operator==(const const_iterator &o) const {
			return pDict == o.pDict && bucketIndex == o.bucketIndex && entry == o.entry;
		}
		bool operator!=(const const_iterator &o) const { return !(*this == o); }

		const_iterator &operator++(){
			if(!entry) return *this;
			entry = entry->next;
			if(entry) return *this;

			++bucketIndex;
			while(pDict && bucketIndex < pDict->pBucketCount){
				entry = pDict->pBuckets[bucketIndex];
				if(entry) return *this;
				++bucketIndex;
			}
			entry = nullptr;
			return *this;
		}

		const_iterator operator++(int){
			const_iterator tmp = *this;
			++*this;
			return tmp;
		}

		value_type operator*() const {
			return value_type(entry->key, entry->value);
		}

	private:
		const decTDictionary *pDict;
		int bucketIndex;
		const sDictEntry *entry;

		void advance_to_valid(){
			if(!pDict){
				entry = nullptr;
				bucketIndex = 0;
				return;
			}
			while(bucketIndex < pDict->pBucketCount && entry == nullptr){
				entry = pDict->pBuckets[bucketIndex];
				if(entry) break;
				++bucketIndex;
			}
			if(bucketIndex >= pDict->pBucketCount){
				entry = nullptr;
				bucketIndex = pDict->pBucketCount;
			}
		}
	}; /* end const_iterator */

	const_iterator cbegin() const { return const_iterator(this, 0, nullptr); }
	const_iterator cend()   const { return const_iterator(this, pBucketCount, nullptr); }

	/* convenience const begin/end */
	const_iterator begin() const { return cbegin(); }
	const_iterator end()   const { return cend(); }
	/*@}*/
	
	
private:
	sDictEntry *pGetEntry(const K &key) const{
		const unsigned int hash = DEHash(key);
		const int bucketIndex = hash % pBucketCount;
		sDictEntry *iterEntry = pBuckets[bucketIndex];
		
		while(iterEntry){
			if(iterEntry->hash == hash && iterEntry->key == key){
				return iterEntry;
			}
			iterEntry = iterEntry->next;
		}
		
		return nullptr;
	}
};


/**
 * \brief Dictionary template class mapping values to string keys.
 * 
 * Convenience typedef for string-keyed dictionaries.
 */
template<typename V, typename VP = V>
using decTStringDictionary = decTDictionary<decString, V, VP>;

#endif

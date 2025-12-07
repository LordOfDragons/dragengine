/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
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

#ifndef _DECUNICODESTRINGDICTIONARY_H_
#define _DECUNICODESTRINGDICTIONARY_H_

#include "decUnicodeString.h"

class decUnicodeStringList;



/**
 * \brief Dictionary of unicode strings mapping strings to string keys.
 */
class decUnicodeStringDictionary{
private:
	struct sDictEntry{
		unsigned int hash;
		decUnicodeString key;
		decUnicodeString value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry(const sDictEntry &entry);
		sDictEntry(unsigned int nhash, const decUnicodeString &nkey, const decUnicodeString &nvalue);
		~sDictEntry();
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new dictionary. */
	decUnicodeStringDictionary();
	/** \brief Creates a new dictionary. */
	decUnicodeStringDictionary(int bucketCount);
	/** \brief Creates a new dictionary. */
	decUnicodeStringDictionary(const decUnicodeStringDictionary &dict);
	/** \brief Cleans up the dictionary. */
	~decUnicodeStringDictionary();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the number of entries in the dictionary. */
	inline int GetCount() const{return pEntryCount;}
	/** \brief Determines if a key is located in the dictionary. */
	bool Has(const decUnicodeString &key) const;
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decUnicodeString &GetAt(const decUnicodeString &key) const;
	/** \brief Retrieves an string by key into a variable returning true if the key has been found or false otherwise. */
	bool GetAt(const decUnicodeString &key, decUnicodeString **string) const;
	/** \brief Adds an string to the dictionary by key replacing an old string if present. */
	void SetAt(const decUnicodeString &key, const decUnicodeString &value);
	/** \brief Removes a key from the dictionary throwing an exception if not found. */
	void Remove(const decUnicodeString &key);
	/** \brief Removes a key from the dictionary if present. */
	void RemoveIfPresent(const decUnicodeString &key);
	/** \brief Removes all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief Retrieves a string list with all keys in the dictionary. */
	decUnicodeStringList GetKeys() const;
	/** \brief Retrieves an string list with all values in the dictionary. */
	decUnicodeStringList GetValues() const;
	
	/** \brief Determines if this dictionary equals another dictionary. */
	bool Equals(const decUnicodeStringDictionary &dict) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determines if this dictionary equals another dictionary. */
	bool operator==(const decUnicodeStringDictionary &dict) const;
	/** \brief Retrieves a new dictionary containing this dictionary and another applied to it. */
	decUnicodeStringDictionary operator+(const decUnicodeStringDictionary &dict) const;
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decUnicodeString &operator[](const decUnicodeString &key) const;
	
	/** \brief Sets this dictionary to another dictionary. */
	decUnicodeStringDictionary &operator=(const decUnicodeStringDictionary &dict);
	/** \brief Applies a dictionary to this dictionary. */
	decUnicodeStringDictionary &operator+=(const decUnicodeStringDictionary &dict);
	/*@}*/
};

#endif

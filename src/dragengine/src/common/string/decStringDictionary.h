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

#ifndef _DECSTRINGDICTIONARY_H_
#define _DECSTRINGDICTIONARY_H_

#include "decString.h"

class decStringList;


/**
 * \brief Dictionary of strings mapping strings to string keys.
 */
class DE_DLL_EXPORT decStringDictionary{
private:
	struct sDictEntry{
		unsigned int hash;
		decString key;
		decString value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry(const sDictEntry &entry);
		sDictEntry(unsigned int nhash, const char *nkey, const char *nvalue);
		~sDictEntry();
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new dictionary. */
	decStringDictionary();
	
	/** \brief Create new dictionary. */
	decStringDictionary(int bucketCount);
	
	/** \brief Create new dictionary. */
	decStringDictionary(const decStringDictionary &dict);
	
	/** \brief Clean up dictionary. */
	~decStringDictionary();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/** \brief Determines if a key is located in the dictionary. */
	bool Has(const char *key) const;
	
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decString &GetAt(const char *key) const;
	
	/** \brief Retrieves an string by key or default value if absent. */
	const decString &GetAt(const char *key, const decString &defaultValue) const;
	
	/** \brief Retrieves an string by key into a variable returning true if the key has been found or false otherwise. */
	bool GetAt(const char *key, const decString **string) const;
	
	/** \brief Adds an string to the dictionary by key replacing an old string if present. */
	void SetAt(const char *key, const char *value);
	
	/** \brief Removes a key from the dictionary throwing an exception if not found. */
	void Remove(const char *key);
	
	/** \brief Removes a key from the dictionary if present. */
	void RemoveIfPresent(const char *key);
	
	/** \brief Removes all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief Retrieves a string list with all keys in the dictionary. */
	decStringList GetKeys() const;
	
	/** \brief Retrieves an string list with all values in the dictionary. */
	decStringList GetValues() const;
	
	/** \brief Determines if this dictionary equals another dictionary. */
	bool Equals(const decStringDictionary &dict) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determines if this dictionary equals another dictionary. */
	bool operator==(const decStringDictionary &dict) const;
	
	/** \brief Retrieves a new dictionary containing this dictionary and another applied to it. */
	decStringDictionary operator+(const decStringDictionary &dict) const;
	
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decString &operator[](const char *key) const;
	
	/** \brief Sets this dictionary to another dictionary. */
	decStringDictionary &operator=(const decStringDictionary &dict);
	
	/** \brief Applies a dictionary to this dictionary. */
	decStringDictionary &operator+=(const decStringDictionary &dict);
	/*@}*/
};

#endif

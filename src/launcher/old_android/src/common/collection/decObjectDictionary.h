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

#ifndef _DECOBJECTDICTIONARY_H_
#define _DECOBJECTDICTIONARY_H_

#include "../string/decString.h"

class decObjectList;
#include <dragengine/deObject.h>



/**
 * \brief Dictionary of objects mapping objects to string keys.
 */
class decObjectDictionary{
private:
	struct sDictEntry{
		unsigned int hash;
		decString key;
		deObject::Ref value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry(const sDictEntry &entry);
		sDictEntry(unsigned int nhash, const char *nkey, deObject *nvalue);
		~sDictEntry();
		void SetValue(deObject *nvalue);
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new dictionary. */
	decObjectDictionary();
	
	/**
	 * \brief Create a new dictionary with initial bucket count.
	 * \throws deeInvalidParam \em bucketCount is less than 1.
	 */
	decObjectDictionary(int bucketCount);
	
	/** \brief Create copy of a dictionary. */
	decObjectDictionary(const decObjectDictionary &dict);
	
	/** \brief Clean up the dictionary. */
	~decObjectDictionary();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/**
	 * \brief Determine if a key is located in the dictionary.
	 * \throws deeInvalidParam \em key is \em NULL.
	 */
	bool Has(const char *key) const;
	
	/**
	 * \brief Object for key.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	deObject *GetAt(const char *key) const;
	
	/**
	 * \brief Object by key
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em object is \em NULL.
	 * \retval true Object of \em key stored in \em object.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt(const char *key, deObject **object) const;
	
	/**
	 * \brief Set key to object.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em value is \em NULL.
	 * \throws deeInvalidParam \em value is an empty string.
	 */
	void SetAt(const char *key, deObject *value);
	
	/**
	 * \brief Remove a key.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void Remove(const char *key);
	
	/**
	 * \brief Remove a key if present in the dictionary.
	 * \throws deeInvalidParam \em key is \em NULL.
	 */
	void RemoveIfPresent(const char *key);
	
	/** \brief Remove all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief List of all keys in the dictionary. */
	decStringList GetKeys() const;
	
	/** \brief List with all values in the dictionary. */
	decObjectList GetValues() const;
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals(const decObjectDictionary &dict) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==(const decObjectDictionary &dict) const;
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decObjectDictionary operator+(const decObjectDictionary &dict) const;
	
	/**
	 * \brief Object for key.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	deObject *operator[](const char *key) const;
	
	/** \brief Copy dictionary to this dictionary. */
	decObjectDictionary &operator=(const decObjectDictionary &dict);
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decObjectDictionary &operator+=(const decObjectDictionary &dict);
	/*@}*/
};

#endif

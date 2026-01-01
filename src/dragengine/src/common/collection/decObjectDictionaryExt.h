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

#ifndef _DECOBJECTDICTIONARYEXT_H_
#define _DECOBJECTDICTIONARYEXT_H_

#include "../../dragengine_export.h"
#include "../../deObject.h"

class decObjectList;
class decPointerList;


/**
 * \brief Dictionary of objects mapping objects to pointer keys.
 * 
 * Extended version of decObjectDictionary using pointer keys with custom hash.
 */
class DE_DLL_EXPORT decObjectDictionaryExt{
private:
	struct sDictEntry{
		unsigned int hash;
		const void *key;
		deObject::Ref value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry(const sDictEntry &entry);
		sDictEntry(unsigned int nhash, const void *nkey, deObject *nvalue);
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
	decObjectDictionaryExt();
	
	/**
	 * \brief Create a new dictionary with initial bucket count.
	 * \throws deeInvalidParam \em bucketCount is less than 1.
	 */
	decObjectDictionaryExt(int bucketCount);
	
	/** \brief Create copy of a dictionary. */
	decObjectDictionaryExt(const decObjectDictionaryExt &dict);
	
	/** \brief Clean up the dictionary. */
	~decObjectDictionaryExt();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/**
	 * \brief Determine if a key is located in the dictionary.
	 * \throws deeInvalidParam \em key is NULL.
	 */
	bool Has(const void *key, unsigned int hash) const;
	
	/**
	 * \brief Object for key.
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	deObject *GetAt(const void *key, unsigned int hash) const;
	
	/**
	 * \brief Object by key
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em object is NULL.
	 * \retval true Object of \em key stored in \em object.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt(const void *key, unsigned int hash, deObject **object) const;
	
	/**
	 * \brief Set key to object.
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em value is NULL.
	 */
	void SetAt(const void *key, unsigned int hash, deObject *value);
	
	/**
	 * \brief Remove a key.
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void Remove(const void *key, unsigned int hash);
	
	/**
	 * \brief Remove a key if present in the dictionary.
	 * \throws deeInvalidParam \em key is NULL.
	 */
	void RemoveIfPresent(const void *key, unsigned int hash);
	
	/** \brief Remove all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief List of all keys in the dictionary. */
	decPointerList GetKeys() const;
	
	/** \brief List with all values in the dictionary. */
	decObjectList GetValues() const;
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals(const decObjectDictionaryExt &dict) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==(const decObjectDictionaryExt &dict) const;
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decObjectDictionaryExt operator+(const decObjectDictionaryExt &dict) const;
	
	/** \brief Copy dictionary to this dictionary. */
	decObjectDictionaryExt &operator=(const decObjectDictionaryExt &dict);
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decObjectDictionaryExt &operator+=(const decObjectDictionaryExt &dict);
	/*@}*/
};

#endif

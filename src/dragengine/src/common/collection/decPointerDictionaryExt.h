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

#ifndef _DECPOINTERDICTIONARYEXT_H_
#define _DECPOINTERDICTIONARYEXT_H_

#include "../../dragengine_export.h"

class decPointerList;


/**
 * \brief Dictionary of pointers mapping objects to pointer keys.
 * 
 * Extended version of decPointerDictionary using pointer keys with custom hash.
 */
class DE_DLL_EXPORT decPointerDictionaryExt{
private:
	struct sDictEntry{
		unsigned int hash;
		const void *key;
		void *value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry(const sDictEntry &entry);
		sDictEntry(unsigned int nhash, const void *nkey, void *nvalue);
		~sDictEntry();
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new dictionary. */
	decPointerDictionaryExt();
	
	/**
	 * \brief Create a new dictionary with initial bucket count.
	 * \throws deeInvalidParam \em bucketCount is less than 1.
	 */
	decPointerDictionaryExt(int bucketCount);
	
	/** \brief Create copy of a dictionary. */
	decPointerDictionaryExt(const decPointerDictionaryExt &dict);
	
	/** \brief Clean up dictionary. */
	~decPointerDictionaryExt();
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
	 * 
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void *GetAt(const void *key, unsigned int hash) const;
	
	/**
	 * \brief Object by key
	 * \throws deeInvalidParam \em key is NULL.
	 * \retval true Object of \em key stored in \em value.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt(const void *key, unsigned int hash, void **value) const;
	
	/**
	 * \brief Set key to object.
	 * \throws deeInvalidParam \em key is NULL.
	 */
	void SetAt(const void *key, unsigned int hash, void *value);
	
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
	decPointerList GetValues() const;
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals(const decPointerDictionaryExt &dict) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==(const decPointerDictionaryExt &dict) const;
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decPointerDictionaryExt operator+(const decPointerDictionaryExt &dict) const;
	
	/** \brief Copy dictionary to this dictionary. */
	decPointerDictionaryExt &operator=(const decPointerDictionaryExt &dict);
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decPointerDictionaryExt &operator+=(const decPointerDictionaryExt &dict);
	/*@}*/
};

#endif

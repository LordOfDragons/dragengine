/* 
 * Drag[en]gine Android Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _DECPOINTERDICTIONARY_H_
#define _DECPOINTERDICTIONARY_H_

#include "../string/decString.h"

class decPointerList;



/**
 * \brief Dictionary of pointers mapping objects to string keys.
 */
class decPointerDictionary{
private:
	struct sDictEntry{
		unsigned int hash;
		decString key;
		void *value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry( const sDictEntry &entry );
		sDictEntry( unsigned int nhash, const char *nkey, void *nvalue );
		~sDictEntry();
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create a new dictionary. */
	decPointerDictionary();
	
	/**
	 * \brief Create a new dictionary with initial bucket count.
	 * \throws deeInvalidParam \em bucketCount is less than 1.
	 */
	decPointerDictionary( int bucketCount );
	
	/** \brief Create copy of a dictionary. */
	decPointerDictionary( const decPointerDictionary &dict );
	
	/** \brief Clean up dictionary. */
	~decPointerDictionary();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/**
	 * \brief Determine if a key is located in the dictionary.
	 * \throws deeInvalidParam \em key is \em NULL.
	 */
	bool Has( const char *key ) const;
	
	/**
	 * \brief Object for key.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void *GetAt( const char *key ) const;
	
	/**
	 * \brief Object by key
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \retval true Object of \em key stored in \em value.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt( const char *key, void **value ) const;
	
	/**
	 * \brief Set key to object.
	 * \throws deeInvalidParam \em key is \em NULL.
	 */
	void SetAt( const char *key, void *value );
	
	/**
	 * \brief Remove a key.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void Remove( const char *key );
	
	/**
	 * \brief Remove a key if present in the dictionary.
	 * \throws deeInvalidParam \em key is \em NULL.
	 */
	void RemoveIfPresent( const char *key );
	
	/** \brief Remove all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief List of all keys in the dictionary. */
	decStringList GetKeys() const;
	
	/** \brief List with all values in the dictionary. */
	decPointerList GetValues() const;
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals( const decPointerDictionary &dict ) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==( const decPointerDictionary &dict ) const;
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decPointerDictionary operator+( const decPointerDictionary &dict ) const;
	
	/**
	 * \brief Object for key.
	 * \throws deeInvalidParam \em key is \em NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void *operator[]( const char *key ) const;
	
	/** \brief Copy dictionary to this dictionary. */
	decPointerDictionary &operator=( const decPointerDictionary &dict );
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decPointerDictionary &operator+=( const decPointerDictionary &dict );
	/*@}*/
};

#endif

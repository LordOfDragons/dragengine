/* 
 * Drag[en]gine Game Engine
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
		sDictEntry( const sDictEntry &entry );
		sDictEntry( unsigned int nhash, const decUnicodeString &nkey, const decUnicodeString &nvalue );
		~sDictEntry();
	};
	
	sDictEntry **pBuckets;
	int pBucketCount;
	int pEntryCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new dictionary. */
	decUnicodeStringDictionary();
	
	/** \brief Create new dictionary. */
	decUnicodeStringDictionary( int bucketCount );
	
	/** \brief Create new dictionary. */
	decUnicodeStringDictionary( const decUnicodeStringDictionary &dict );
	
	/** \brief Clean up dictionary. */
	~decUnicodeStringDictionary();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/** \brief Determines if a key is located in the dictionary. */
	bool Has( const decUnicodeString &key ) const;
	
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decUnicodeString &GetAt( const decUnicodeString &key ) const;
	
	/** \brief Retrieves an string by key into a variable returning true if the key has been found or false otherwise. */
	bool GetAt( const decUnicodeString &key, decUnicodeString **string ) const;
	
	/** \brief Adds an string to the dictionary by key replacing an old string if present. */
	void SetAt( const decUnicodeString &key, const decUnicodeString &value );
	
	/** \brief Removes a key from the dictionary throwing an exception if not found. */
	void Remove( const decUnicodeString &key );
	
	/** \brief Removes a key from the dictionary if present. */
	void RemoveIfPresent( const decUnicodeString &key );
	
	/** \brief Removes all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief Retrieves a string list with all keys in the dictionary. */
	decUnicodeStringList GetKeys() const;
	
	/** \brief Retrieves an string list with all values in the dictionary. */
	decUnicodeStringList GetValues() const;
	
	/** \brief Determines if this dictionary equals another dictionary. */
	bool Equals( const decUnicodeStringDictionary &dict ) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determines if this dictionary equals another dictionary. */
	bool operator==( const decUnicodeStringDictionary &dict ) const;
	
	/** \brief Retrieves a new dictionary containing this dictionary and another applied to it. */
	decUnicodeStringDictionary operator+( const decUnicodeStringDictionary &dict ) const;
	
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decUnicodeString &operator[]( const decUnicodeString &key ) const;
	
	/** \brief Sets this dictionary to another dictionary. */
	decUnicodeStringDictionary &operator=( const decUnicodeStringDictionary &dict );
	
	/** \brief Applies a dictionary to this dictionary. */
	decUnicodeStringDictionary &operator+=( const decUnicodeStringDictionary &dict );
	/*@}*/
};

#endif

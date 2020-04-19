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

#ifndef _DECSTRINGDICTIONARY_H_
#define _DECSTRINGDICTIONARY_H_

#include "decString.h"

class decStringList;


/**
 * \brief Dictionary of strings mapping strings to string keys.
 */
class decStringDictionary{
private:
	struct sDictEntry{
		unsigned int hash;
		decString key;
		decString value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry( const sDictEntry &entry );
		sDictEntry( unsigned int nhash, const char *nkey, const char *nvalue );
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
	decStringDictionary( int bucketCount );
	
	/** \brief Create new dictionary. */
	decStringDictionary( const decStringDictionary &dict );
	
	/** \brief Clean up dictionary. */
	~decStringDictionary();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of entries in the dictionary. */
	inline int GetCount() const{ return pEntryCount; }
	
	/** \brief Determines if a key is located in the dictionary. */
	bool Has( const char *key ) const;
	
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decString &GetAt( const char *key ) const;
	
	/** \brief Retrieves an string by key or default value if absent. */
	const decString &GetAt( const char *key, const decString &defaultValue ) const;
	
	/** \brief Retrieves an string by key into a variable returning true if the key has been found or false otherwise. */
	bool GetAt( const char *key, const decString **string ) const;
	
	/** \brief Adds an string to the dictionary by key replacing an old string if present. */
	void SetAt( const char *key, const char *value );
	
	/** \brief Removes a key from the dictionary throwing an exception if not found. */
	void Remove( const char *key );
	
	/** \brief Removes a key from the dictionary if present. */
	void RemoveIfPresent( const char *key );
	
	/** \brief Removes all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief Retrieves a string list with all keys in the dictionary. */
	decStringList GetKeys() const;
	
	/** \brief Retrieves an string list with all values in the dictionary. */
	decStringList GetValues() const;
	
	/** \brief Determines if this dictionary equals another dictionary. */
	bool Equals( const decStringDictionary &dict ) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determines if this dictionary equals another dictionary. */
	bool operator==( const decStringDictionary &dict ) const;
	
	/** \brief Retrieves a new dictionary containing this dictionary and another applied to it. */
	decStringDictionary operator+( const decStringDictionary &dict ) const;
	
	/** \brief Retrieves an string by key throwing an exception if not found. */
	const decString &operator[]( const char *key ) const;
	
	/** \brief Sets this dictionary to another dictionary. */
	decStringDictionary &operator=( const decStringDictionary &dict );
	
	/** \brief Applies a dictionary to this dictionary. */
	decStringDictionary &operator+=( const decStringDictionary &dict );
	/*@}*/
};

#endif

/* 
 * Drag[en]gine Game Engine
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#ifndef _DECOBJECTDICTIONARYEXT_H_
#define _DECOBJECTDICTIONARYEXT_H_

class deObject;
class decObjectList;
class decPointerList;


/**
 * \brief Dictionary of objects mapping objects to pointer keys.
 * 
 * Extended version of decObjectDictionary using pointer keys with custom hash.
 */
class decObjectDictionaryExt{
private:
	struct sDictEntry{
		unsigned int hash;
		const void *key;
		deObject *value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry( const sDictEntry &entry );
		sDictEntry( unsigned int nhash, const void *nkey, deObject *nvalue );
		~sDictEntry();
		void SetValue( deObject *nvalue );
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
	decObjectDictionaryExt( int bucketCount );
	
	/** \brief Create copy of a dictionary. */
	decObjectDictionaryExt( const decObjectDictionaryExt &dict );
	
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
	bool Has( const void *key, unsigned int hash ) const;
	
	/**
	 * \brief Object for key.
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	deObject *GetAt( const void *key, unsigned int hash ) const;
	
	/**
	 * \brief Object by key
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em object is NULL.
	 * \retval true Object of \em key stored in \em object.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt( const void *key, unsigned int hash, deObject **object ) const;
	
	/**
	 * \brief Set key to object.
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em value is NULL.
	 */
	void SetAt( const void *key, unsigned int hash, deObject *value );
	
	/**
	 * \brief Remove a key.
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void Remove( const void *key, unsigned int hash );
	
	/**
	 * \brief Remove a key if present in the dictionary.
	 * \throws deeInvalidParam \em key is NULL.
	 */
	void RemoveIfPresent( const void *key, unsigned int hash );
	
	/** \brief Remove all keys from the dictionary. */
	void RemoveAll();
	
	/** \brief List of all keys in the dictionary. */
	decPointerList GetKeys() const;
	
	/** \brief List with all values in the dictionary. */
	decObjectList GetValues() const;
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals( const decObjectDictionaryExt &dict ) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==( const decObjectDictionaryExt &dict ) const;
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decObjectDictionaryExt operator+( const decObjectDictionaryExt &dict ) const;
	
	/** \brief Copy dictionary to this dictionary. */
	decObjectDictionaryExt &operator=( const decObjectDictionaryExt &dict );
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decObjectDictionaryExt &operator+=( const decObjectDictionaryExt &dict );
	/*@}*/
};

#endif

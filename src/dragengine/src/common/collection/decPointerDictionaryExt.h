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

#ifndef _DECPOINTERDICTIONARYEXT_H_
#define _DECPOINTERDICTIONARYEXT_H_

class decPointerList;


/**
 * \brief Dictionary of pointers mapping objects to pointer keys.
 * 
 * Extended version of decPointerDictionary using pointer keys with custom hash.
 */
class decPointerDictionaryExt{
private:
	struct sDictEntry{
		unsigned int hash;
		const void *key;
		void *value;
		sDictEntry *next;
		
		sDictEntry();
		sDictEntry( const sDictEntry &entry );
		sDictEntry( unsigned int nhash, const void *nkey, void *nvalue );
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
	decPointerDictionaryExt( int bucketCount );
	
	/** \brief Create copy of a dictionary. */
	decPointerDictionaryExt( const decPointerDictionaryExt &dict );
	
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
	bool Has( const void *key, unsigned int hash ) const;
	
	/**
	 * \brief Object for key.
	 * 
	 * \throws deeInvalidParam \em key is NULL.
	 * \throws deeInvalidParam \em key is not present in the dictionary.
	 */
	void *GetAt( const void *key, unsigned int hash ) const;
	
	/**
	 * \brief Object by key
	 * \throws deeInvalidParam \em key is NULL.
	 * \retval true Object of \em key stored in \em value.
	 * \retval false \em key is not present in the dictionary.
	 */
	bool GetAt( const void *key, unsigned int hash, void **value ) const;
	
	/**
	 * \brief Set key to object.
	 * \throws deeInvalidParam \em key is NULL.
	 */
	void SetAt( const void *key, unsigned int hash, void *value );
	
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
	decPointerList GetValues() const;
	
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool Equals( const decPointerDictionaryExt &dict ) const;
	
	/** \brief Check load of the dictionary changing bucket count if required. */
	void CheckLoad();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Determine if dictionary is equal to another dictionary. */
	bool operator==( const decPointerDictionaryExt &dict ) const;
	
	/** \brief New dictionary containing keys of this dictionary and the keys of another applied ontop of it. */
	decPointerDictionaryExt operator+( const decPointerDictionaryExt &dict ) const;
	
	/** \brief Copy dictionary to this dictionary. */
	decPointerDictionaryExt &operator=( const decPointerDictionaryExt &dict );
	
	/** \brief Set all keys from dictionary to this dictionary. */
	decPointerDictionaryExt &operator+=( const decPointerDictionaryExt &dict );
	/*@}*/
};

#endif

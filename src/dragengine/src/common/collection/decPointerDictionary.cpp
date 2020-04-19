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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decPointerDictionary.h"
#include "decPointerList.h"

#include "../exceptions.h"
#include "../string/decStringList.h"



// Struct decPointerDictionary::sDictEntry
////////////////////////////////////////////

decPointerDictionary::sDictEntry::sDictEntry() :
hash( 0 ),
value( NULL ),
next( NULL ){
}

decPointerDictionary::sDictEntry::sDictEntry( const decPointerDictionary::sDictEntry &entry ) :
hash( entry.hash ),
key( entry.key ),
value( entry.value ),
next( NULL ){
}

decPointerDictionary::sDictEntry::sDictEntry( unsigned int nhash, const char *nkey, void *nvalue ) :
hash( nhash ),
key( nkey ),
value( nvalue ),
next( NULL ){
}

decPointerDictionary::sDictEntry::~sDictEntry(){
	value = NULL;
	next = NULL;
}



// Class decPointerDictionary
//////////////////////////////

// Constructor, destructor
////////////////////////////

decPointerDictionary::decPointerDictionary(){
	pBuckets = NULL;
	pBucketCount = 8;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[ pBucketCount ];
	
	int i;
	for( i=0; i<pBucketCount; i++ ){
		pBuckets[ i ] = NULL;
	}
}

decPointerDictionary::decPointerDictionary( int bucketCount ){
	if( bucketCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pBuckets = NULL;
	pBucketCount = bucketCount;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[ bucketCount ];
	
	int i;
	for( i=0; i<bucketCount; i++ ){
		pBuckets[ i ] = NULL;
	}
}

decPointerDictionary::decPointerDictionary( const decPointerDictionary &dict ){
	pBuckets = NULL;
	pBucketCount = dict.pBucketCount;
	pEntryCount = dict.pEntryCount;
	
	pBuckets = new sDictEntry*[ pBucketCount ];
	
	int i;
	for( i=0; i<pBucketCount; i++ ){
		pBuckets[ i ] = NULL;
	}
	
	sDictEntry *newEntry;
	try{
		for( i=0; i<pBucketCount; i++ ){
			sDictEntry *iterEntry = dict.pBuckets[ i ];
			sDictEntry *lastEntry = NULL;
			newEntry = NULL;
			
			while( iterEntry ){
				newEntry = new sDictEntry( *iterEntry );
				
				if( lastEntry ){
					lastEntry->next = newEntry;
					
				}else{
					pBuckets[ i ] = newEntry;
				}
				lastEntry = newEntry;
				
				iterEntry = iterEntry->next;
			}
		}
		
	}catch( const deException & ){
		if( newEntry ){
			delete newEntry;
		}
		throw;
	}
}

decPointerDictionary::~decPointerDictionary(){
	RemoveAll();
	
	if( pBuckets ){
		delete [] pBuckets;
		pBuckets = NULL;
	}
	
	pBucketCount = 0;
}



// Management
///////////////

bool decPointerDictionary::Has( const char *key ) const{
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const unsigned int hash = decString::Hash( key );
	
	sDictEntry *iterEntry = pBuckets[ hash % pBucketCount ];
	
	while( iterEntry ){
		if( iterEntry->hash == hash && iterEntry->key == key ){
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void *decPointerDictionary::GetAt( const char *key ) const{
	void *object;
	
	if( ! GetAt( key, &object ) ){
		DETHROW( deeInvalidParam );
	}
	
	return object;
}

bool decPointerDictionary::GetAt( const char *key, void **object ) const{
	if( ! key || ! object ){
		DETHROW( deeNullPointer );
	}
	
	const unsigned int hash = decString::Hash( key );
	
	sDictEntry *iterEntry = pBuckets[ hash % pBucketCount ];
	
	while( iterEntry ){
		if( iterEntry->hash == hash ){
			if( iterEntry->key == key ){
				*object = iterEntry->value;
				return true;
			}
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void decPointerDictionary::SetAt( const char *key, void *value ){
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const unsigned int hash = decString::Hash( key );
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->hash == hash && iterEntry->key == key ){
			iterEntry->value = value;
			pEntryCount++;
			CheckLoad();
			return;
		}
		lastEntry = iterEntry;
		iterEntry = iterEntry->next;
	}
	
	sDictEntry *newEntry = NULL;
	try{
		newEntry = new sDictEntry(hash, key, value );
		
		if( lastEntry ){
			lastEntry->next = newEntry;
			
		}else{
			pBuckets[ bucketIndex ] = newEntry;
		}
		
	}catch( const deException & ){
		if( newEntry ){
			delete newEntry;
		}
		throw;
	}
	
	pEntryCount++;
	CheckLoad();
}

void decPointerDictionary::Remove( const char *key ){
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const unsigned int hash = decString::Hash( key );
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->hash == hash && iterEntry->key == key ){
			if( lastEntry ){
				lastEntry->next = iterEntry->next;
				
			}else{
				pBuckets[ bucketIndex ] = iterEntry->next;
			}
			
			pEntryCount--;
			
			delete iterEntry;
			
			return;
		}
		lastEntry = iterEntry;
		iterEntry = iterEntry->next;
	}
	
	DETHROW( deeInvalidParam );
}

void decPointerDictionary::RemoveIfPresent( const char *key ){
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const unsigned int hash = decString::Hash( key );
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->hash == hash && iterEntry->key == key ){
			if( lastEntry ){
				lastEntry->next = iterEntry->next;
				
			}else{
				pBuckets[ bucketIndex ] = iterEntry->next;
			}
			
			pEntryCount--;
			
			delete iterEntry;
			
			return;
		}
		lastEntry = iterEntry;
		iterEntry = iterEntry->next;
	}
}

void decPointerDictionary::RemoveAll(){
	if( pBuckets ){
		int i;
		
		for( i=0; i<pBucketCount; i++ ){
			if( pBuckets[ i ] ){
				sDictEntry *iterEntry = pBuckets[ i ];
				pBuckets[ i ] = NULL;
				
				while( iterEntry ){
					sDictEntry * const delbucket = iterEntry;
					iterEntry = iterEntry->next;
					delete delbucket;
				}
			}
		}
	}
	
	pEntryCount = 0;
}



decStringList decPointerDictionary::GetKeys() const{
	decStringList keys;
	int i;
	
	for( i=0; i<pBucketCount; i++ ){
		sDictEntry *iterEntry = pBuckets[ i ];
		
		while( iterEntry ){
			keys.Add( iterEntry->key );
			iterEntry = iterEntry->next;
		}
	}
	
	return keys;
}

decPointerList decPointerDictionary::GetValues() const{
	decPointerList values;
	int i;
	
	for( i=0; i<pBucketCount; i++ ){
		sDictEntry *iterEntry = pBuckets[ i ];
		
		while( iterEntry ){
			values.Add( iterEntry->value );
			iterEntry = iterEntry->next;
		}
	}
	
	return values;
}



bool decPointerDictionary::Equals( const decPointerDictionary &dict ) const{
	void *object;
	int i;
	
	if( dict.pEntryCount != pEntryCount ){
		return false;
	}
	
	for( i=0; i<pBucketCount; i++ ){
		sDictEntry *iterEntry = pBuckets[ i ];
		
		while( iterEntry ){
			if( ! dict.GetAt( iterEntry->key, &object ) || object != iterEntry->value ){
				return false;
			}
			iterEntry = iterEntry->next;
		}
	}
	
	return true;
}



void decPointerDictionary::CheckLoad(){
	if( ( float )pEntryCount / ( float )pBucketCount > 0.7 ){
		const int newBucketCount = pBucketCount + ( pBucketCount >> 1 ); // +50%
		sDictEntry ** const newBuckets = new sDictEntry*[ newBucketCount ];
		int i;
		
		if( ! newBuckets ){
			DETHROW( deeInvalidParam );
		}
		for( i=0; i<newBucketCount; i++ ){
			newBuckets[ i ] = NULL;
		}
		
		for( i=0; i<pBucketCount; i++ ){
			sDictEntry *iterEntry = pBuckets[ i ];
			
			while( iterEntry ){
				sDictEntry * const moveEntry = iterEntry;
				iterEntry = iterEntry->next;
				
				const int bucketIndex = ( moveEntry->hash % newBucketCount );
				sDictEntry *iterEntry2 = newBuckets[ bucketIndex ];
				
				if( iterEntry2 ){
					while( iterEntry2->next ){
						iterEntry2 = iterEntry2->next;
					}
					iterEntry2->next = moveEntry;
					
				}else{
					newBuckets[ bucketIndex ] = moveEntry;
				}
				
				moveEntry->next = NULL;
			}
		}
		
		//printf( "Dictionary grows from %i buckets to %i buckets\n", nd.bucketCount, newBucketCount );
		delete [] pBuckets;
		pBuckets = newBuckets;
		pBucketCount = newBucketCount;
	}
}



// Operators
//////////////

bool decPointerDictionary::operator==( const decPointerDictionary &dict ) const{
	return Equals( dict );
}

decPointerDictionary decPointerDictionary::operator+( const decPointerDictionary &dict ) const{
	decPointerDictionary ndict( *this );
	int i;
	
	for( i=0; i<dict.pBucketCount; i++ ){
		sDictEntry *iterEntry = dict.pBuckets[ i ];
		
		while( iterEntry ){
			ndict.SetAt( iterEntry->key, iterEntry->value );
			iterEntry = iterEntry->next;
		}
	}
	
	return ndict;
}

void *decPointerDictionary::operator[]( const char *key ) const{
	return GetAt( key );
}



decPointerDictionary &decPointerDictionary::operator=( const decPointerDictionary &dict ){
	RemoveAll();
	return *this += dict;
}

decPointerDictionary &decPointerDictionary::operator+=( const decPointerDictionary &dict ){
	int i;
	
	for( i=0; i<dict.pBucketCount; i++ ){
		sDictEntry *iterEntry = dict.pBuckets[ i ];
		
		while( iterEntry ){
			SetAt( iterEntry->key, iterEntry->value );
			iterEntry = iterEntry->next;
		}
	}
	
	return *this;
}

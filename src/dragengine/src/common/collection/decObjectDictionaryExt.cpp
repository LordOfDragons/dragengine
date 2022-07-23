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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decObjectDictionaryExt.h"
#include "decObjectList.h"
#include "decPointerList.h"
#include "../exceptions.h"
#include "../../deObject.h"



// Struct decObjectDictionaryExt::sDictEntry
//////////////////////////////////////////////

decObjectDictionaryExt::sDictEntry::sDictEntry() :
hash( 0 ),
value( NULL ),
next( NULL ){
}

decObjectDictionaryExt::sDictEntry::sDictEntry( const decObjectDictionaryExt::sDictEntry &entry ) :
hash( entry.hash ),
key( entry.key ),
value( entry.value ),
next( NULL )
{
	if( value ){
		value->AddReference();
	}
}

decObjectDictionaryExt::sDictEntry::sDictEntry( unsigned int nhash, const void *nkey, deObject *nvalue ) :
hash( nhash ),
key( nkey ),
value( nvalue ),
next( NULL )
{
	if( value ){
		value->AddReference();
	}
}

decObjectDictionaryExt::sDictEntry::~sDictEntry(){
	SetValue( NULL );
	next = NULL;
}

void decObjectDictionaryExt::sDictEntry::SetValue( deObject *nvalue ){
	if( nvalue != value ){
		if( value ){
			value->FreeReference();
		}
		
		value = nvalue;
		
		if( nvalue ){
			nvalue->AddReference();
		}
	}
}



// Class decObjectDictionaryExt
//////////////////////////////

// Constructor, destructor
////////////////////////////

decObjectDictionaryExt::decObjectDictionaryExt(){
	pBuckets = NULL;
	pBucketCount = 8;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[ pBucketCount ];
	
	int i;
	for( i=0; i<pBucketCount; i++ ){
		pBuckets[ i ] = NULL;
	}
}

decObjectDictionaryExt::decObjectDictionaryExt( int bucketCount ){
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

decObjectDictionaryExt::decObjectDictionaryExt( const decObjectDictionaryExt &dict ){
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

decObjectDictionaryExt::~decObjectDictionaryExt(){
	RemoveAll();
	
	if( pBuckets ){
		delete [] pBuckets;
		pBuckets = NULL;
	}
	
	pBucketCount = 0;
}



// Management
///////////////

bool decObjectDictionaryExt::Has( const void *key, unsigned int hash ) const{
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	sDictEntry *iterEntry = pBuckets[ hash % pBucketCount ];
	
	while( iterEntry ){
		if( iterEntry->key == key ){
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

deObject *decObjectDictionaryExt::GetAt( const void *key, unsigned int hash ) const{
	deObject *object;
	
	if( ! GetAt( key, hash, &object ) ){
		DETHROW( deeInvalidParam );
	}
	
	return object;
}

bool decObjectDictionaryExt::GetAt( const void *key, unsigned int hash, deObject **object ) const{
	if( ! key || ! object ){
		DETHROW( deeNullPointer );
	}
	
	sDictEntry *iterEntry = pBuckets[ hash % pBucketCount ];
	
	while( iterEntry ){
		if( iterEntry->key == key ){
			*object = iterEntry->value;
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void decObjectDictionaryExt::SetAt( const void *key, unsigned int hash, deObject *value ){
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->key == key ){
			iterEntry->SetValue( value );
			pEntryCount++;
			CheckLoad();
			return;
		}
		lastEntry = iterEntry;
		iterEntry = iterEntry->next;
	}
	
	sDictEntry *newEntry = NULL;
	try{
		newEntry = new sDictEntry( hash, key, value );
		
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

void decObjectDictionaryExt::Remove( const void *key, unsigned int hash ){
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->key == key ){
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

void decObjectDictionaryExt::RemoveIfPresent( const void *key, unsigned int hash ){
	if( ! key ){
		DETHROW( deeNullPointer );
	}
	
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->key == key ){
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

void decObjectDictionaryExt::RemoveAll(){
	if( pEntryCount == 0 ){
		return;
	}
	
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



decPointerList decObjectDictionaryExt::GetKeys() const{
	decPointerList keys;
	int i;
	
	for( i=0; i<pBucketCount; i++ ){
		sDictEntry *iterEntry = pBuckets[ i ];
		
		while( iterEntry ){
			keys.Add( ( void* )iterEntry->key ); // this is bad, I know, but necessary
			iterEntry = iterEntry->next;
		}
	}
	
	return keys;
}

decObjectList decObjectDictionaryExt::GetValues() const{
	decObjectList values;
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



bool decObjectDictionaryExt::Equals( const decObjectDictionaryExt &dict ) const{
	deObject *object;
	int i;
	
	if( dict.pEntryCount != pEntryCount ){
		return false;
	}
	
	for( i=0; i<pBucketCount; i++ ){
		sDictEntry *iterEntry = pBuckets[ i ];
		
		while( iterEntry ){
			if( ! dict.GetAt( iterEntry->key, iterEntry->hash, &object ) || object != iterEntry->value ){
				return false;
			}
			iterEntry = iterEntry->next;
		}
	}
	
	return true;
}



void decObjectDictionaryExt::CheckLoad(){
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

bool decObjectDictionaryExt::operator==( const decObjectDictionaryExt &dict ) const{
	return Equals( dict );
}

decObjectDictionaryExt decObjectDictionaryExt::operator+( const decObjectDictionaryExt &dict ) const{
	decObjectDictionaryExt ndict( *this );
	int i;
	
	for( i=0; i<dict.pBucketCount; i++ ){
		sDictEntry *iterEntry = dict.pBuckets[ i ];
		
		while( iterEntry ){
			ndict.SetAt( iterEntry->key, iterEntry->hash, iterEntry->value );
			iterEntry = iterEntry->next;
		}
	}
	
	return ndict;
}



decObjectDictionaryExt &decObjectDictionaryExt::operator=( const decObjectDictionaryExt &dict ){
	RemoveAll();
	return *this += dict;
}

decObjectDictionaryExt &decObjectDictionaryExt::operator+=( const decObjectDictionaryExt &dict ){
	int i;
	
	for( i=0; i<dict.pBucketCount; i++ ){
		sDictEntry *iterEntry = dict.pBuckets[ i ];
		
		while( iterEntry ){
			SetAt( iterEntry->key, iterEntry->hash, iterEntry->value );
			iterEntry = iterEntry->next;
		}
	}
	
	return *this;
}

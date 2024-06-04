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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decStringDictionary.h"
#include "decStringList.h"

#include "../exceptions.h"



// Struct decStringDictionary::sDictEntry
///////////////////////////////////////////

decStringDictionary::sDictEntry::sDictEntry() :
hash( 0 ){
}

decStringDictionary::sDictEntry::sDictEntry( const decStringDictionary::sDictEntry &entry ) :
hash( entry.hash ),
key( entry.key ),
value( entry.value ),
next( NULL ){
}

decStringDictionary::sDictEntry::sDictEntry( unsigned int nhash, const char *nkey,
const char *nvalue ) :
hash( nhash ),
key( nkey ),
value( nvalue ),
next( NULL ){
}

decStringDictionary::sDictEntry::~sDictEntry(){
	next = NULL;
}



// Class decStringDictionary
//////////////////////////////

// Constructor, destructor
////////////////////////////

decStringDictionary::decStringDictionary(){
	pBuckets = NULL;
	pBucketCount = 8;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[ pBucketCount ];
	
	int i;
	for( i=0; i<pBucketCount; i++ ){
		pBuckets[ i ] = NULL;
	}
}

decStringDictionary::decStringDictionary( int bucketCount ){
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

decStringDictionary::decStringDictionary( const decStringDictionary &dict ){
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

decStringDictionary::~decStringDictionary(){
	RemoveAll();
	
	if( pBuckets ){
		delete [] pBuckets;
		pBuckets = NULL;
	}
	
	pBucketCount = 0;
}



// Management
///////////////

bool decStringDictionary::Has( const char *key ) const{
	if( ! key ){
		DETHROW( deeInvalidParam );
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

const decString &decStringDictionary::GetAt( const char *key ) const{
	const decString *string;
	if( ! GetAt( key, &string ) ){
		DETHROW( deeInvalidParam );
	}
	return *string;
}

const decString &decStringDictionary::GetAt( const char *key, const decString &defaultValue ) const{
	const decString *string;
	if( GetAt( key, &string ) ){
		return *string;
	}
	return defaultValue;
}

bool decStringDictionary::GetAt( const char *key, const decString **string ) const{
	if( ! key || ! string ){
		DETHROW( deeInvalidParam );
	}
	
	const unsigned int hash = decString::Hash( key );
	
	sDictEntry *iterEntry = pBuckets[ hash % pBucketCount ];
	
	while( iterEntry ){
		if( iterEntry->hash == hash && iterEntry->key == key ){
			*string = &iterEntry->value;
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void decStringDictionary::SetAt( const char *key, const char *value ){
	if( ! key || key[ 0 ] == '\0' ){
		DETHROW( deeInvalidParam);
	}
	
	const unsigned int hash = decString::Hash( key );
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[ bucketIndex ];
	sDictEntry *lastEntry = NULL;
	
	while( iterEntry ){
		if( iterEntry->hash == hash && iterEntry->key == key ){
			iterEntry->value = value;
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

void decStringDictionary::Remove( const char *key ){
	if( ! key ){
		DETHROW( deeInvalidParam );
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

void decStringDictionary::RemoveIfPresent( const char *key ){
	if( ! key ){
		DETHROW( deeInvalidParam );
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

void decStringDictionary::RemoveAll(){
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



decStringList decStringDictionary::GetKeys() const{
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

decStringList decStringDictionary::GetValues() const{
	decStringList values;
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



bool decStringDictionary::Equals( const decStringDictionary &dict ) const{
	const decString *string;
	int i;
	
	if( dict.pEntryCount != pEntryCount ){
		return false;
	}
	
	for( i=0; i<pBucketCount; i++ ){
		sDictEntry *iterEntry = pBuckets[ i ];
		
		while( iterEntry ){
			if( ! dict.GetAt( iterEntry->key, &string ) || *string != iterEntry->value ){
				return false;
			}
			iterEntry = iterEntry->next;
		}
	}
	
	return true;
}



void decStringDictionary::CheckLoad(){
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

bool decStringDictionary::operator==( const decStringDictionary &dict ) const{
	return Equals( dict );
}

decStringDictionary decStringDictionary::operator+( const decStringDictionary &dict ) const{
	decStringDictionary ndict( *this );
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

const decString &decStringDictionary::operator[]( const char *key ) const{
	return GetAt( key );
}



decStringDictionary &decStringDictionary::operator=( const decStringDictionary &dict ){
	RemoveAll();
	return *this += dict;
}

decStringDictionary &decStringDictionary::operator+=( const decStringDictionary &dict ){
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

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

#include "decUnicodeStringDictionary.h"
#include "decUnicodeStringList.h"

#include "../../exceptions.h"



// Struct decUnicodeStringDictionary::sDictEntry
//////////////////////////////////////////////////

decUnicodeStringDictionary::sDictEntry::sDictEntry() :
hash(0),
next(NULL)
{
	hash = 0;
	next = NULL;
}

decUnicodeStringDictionary::sDictEntry::sDictEntry(
const decUnicodeStringDictionary::sDictEntry &entry) :
hash(entry.hash),
key(entry.key),
value(entry.value),
next(NULL){
}

decUnicodeStringDictionary::sDictEntry::sDictEntry(unsigned int nhash,
const decUnicodeString &nkey, const decUnicodeString &nvalue) :
hash(nhash),
key(nkey),
value(nvalue),
next(NULL){
}

decUnicodeStringDictionary::sDictEntry::~sDictEntry(){
	next = NULL;
}



// Class decUnicodeStringDictionary
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

decUnicodeStringDictionary::decUnicodeStringDictionary(){
	pBuckets = NULL;
	pBucketCount = 8;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[pBucketCount];
	
	int i;
	for(i=0; i<pBucketCount; i++){
		pBuckets[i] = NULL;
	}
}

decUnicodeStringDictionary::decUnicodeStringDictionary(int bucketCount){
	if(bucketCount < 1){
		DETHROW(deeInvalidParam);
	}
	
	pBuckets = NULL;
	pBucketCount = bucketCount;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[bucketCount];
	
	int i;
	for(i=0; i<bucketCount; i++){
		pBuckets[i] = NULL;
	}
}

decUnicodeStringDictionary::decUnicodeStringDictionary(const decUnicodeStringDictionary &dict){
	pBuckets = NULL;
	pBucketCount = dict.pBucketCount;
	pEntryCount = dict.pEntryCount;
	
	pBuckets = new sDictEntry*[pBucketCount];
	
	int i;
	for(i=0; i<pBucketCount; i++){
		pBuckets[i] = NULL;
	}
	
	sDictEntry *newEntry;
	try{
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = dict.pBuckets[i];
			sDictEntry *lastEntry = NULL;
			newEntry = NULL;
			
			while(iterEntry){
				newEntry = new sDictEntry(*iterEntry);
				
				if(lastEntry){
					lastEntry->next = newEntry;
					
				}else{
					pBuckets[i] = newEntry;
				}
				lastEntry = newEntry;
				
				iterEntry = iterEntry->next;
			}
		}
		
	}catch(const deException &){
		if(newEntry){
			delete newEntry;
		}
		throw;
	}
}

decUnicodeStringDictionary::~decUnicodeStringDictionary(){
	RemoveAll();
	
	if(pBuckets){
		delete [] pBuckets;
		pBuckets = NULL;
	}
	
	pBucketCount = 0;
}



// Management
///////////////

bool decUnicodeStringDictionary::Has(const decUnicodeString &key) const{
	if(! key){
		DETHROW(deeInvalidParam);
	}
	
	const unsigned int hash = key.Hash();
	
	sDictEntry *iterEntry = pBuckets[hash % pBucketCount];
	
	while(iterEntry){
		if(iterEntry->hash == hash && iterEntry->key == key){
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

const decUnicodeString &decUnicodeStringDictionary::GetAt(const decUnicodeString &key) const{
	decUnicodeString *string;
	if(! GetAt(key,&string)){
		DETHROW(deeInvalidParam);
	}
	return *string;
}

bool decUnicodeStringDictionary::GetAt(const decUnicodeString &key, decUnicodeString **string) const{
	if(! key || ! string){
		DETHROW(deeInvalidParam);
	}
	
	const unsigned int hash = key.Hash();
	
	sDictEntry *iterEntry = pBuckets[hash % pBucketCount];
	
	while(iterEntry){
		if(iterEntry->hash == hash){
			if(iterEntry->key == key){
				*string = &iterEntry->value;
				return true;
			}
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void decUnicodeStringDictionary::SetAt(const decUnicodeString &key, const decUnicodeString &value){
	if(! key || key[0] == '\0'){
		DETHROW(deeInvalidParam);
	}
	
	const unsigned int hash = key.Hash();
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[bucketIndex];
	sDictEntry *lastEntry = NULL;
	
	while(iterEntry){
		if(iterEntry->hash == hash && iterEntry->key == key){
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
		newEntry = new sDictEntry(hash, key, value);
		
		if(lastEntry){
			lastEntry->next = newEntry;
			
		}else{
			pBuckets[bucketIndex] = newEntry;
		}
		
	}catch(const deException &){
		if(newEntry){
			delete newEntry;
		}
		throw;
	}
	
	pEntryCount++;
	CheckLoad();
}

void decUnicodeStringDictionary::Remove(const decUnicodeString &key){
	if(! key){
		DETHROW(deeInvalidParam);
	}
	
	const unsigned int hash = key.Hash();
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[bucketIndex];
	sDictEntry *lastEntry = NULL;
	
	while(iterEntry){
		if(iterEntry->hash == hash && iterEntry->key == key){
			if(lastEntry){
				lastEntry->next = iterEntry->next;
				
			}else{
				pBuckets[bucketIndex] = iterEntry->next;
			}
			
			pEntryCount--;
			
			delete iterEntry;
			
			return;
		}
		lastEntry = iterEntry;
		iterEntry = iterEntry->next;
	}
	
	DETHROW(deeInvalidParam);
}

void decUnicodeStringDictionary::RemoveIfPresent(const decUnicodeString &key){
	if(! key){
		DETHROW(deeInvalidParam);
	}
	
	const unsigned int hash = key.Hash();
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[bucketIndex];
	sDictEntry *lastEntry = NULL;
	
	while(iterEntry){
		if(iterEntry->hash == hash && iterEntry->key == key){
			if(lastEntry){
				lastEntry->next = iterEntry->next;
				
			}else{
				pBuckets[bucketIndex] = iterEntry->next;
			}
			
			pEntryCount--;
			
			delete iterEntry;
			
			return;
		}
		lastEntry = iterEntry;
		iterEntry = iterEntry->next;
	}
}

void decUnicodeStringDictionary::RemoveAll(){
	if(pBuckets){
		int i;
		
		for(i=0; i<pBucketCount; i++){
			if(pBuckets[i]){
				sDictEntry *iterEntry = pBuckets[i];
				pBuckets[i] = NULL;
				
				while(iterEntry){
					sDictEntry * const delbucket = iterEntry;
					iterEntry = iterEntry->next;
					delete delbucket;
				}
			}
		}
	}
	
	pEntryCount = 0;
}



decUnicodeStringList decUnicodeStringDictionary::GetKeys() const{
	decUnicodeStringList keys;
	int i;
	
	for(i=0; i<pBucketCount; i++){
		sDictEntry *iterEntry = pBuckets[i];
		
		while(iterEntry){
			keys.Add(iterEntry->key);
			iterEntry = iterEntry->next;
		}
	}
	
	return keys;
}

decUnicodeStringList decUnicodeStringDictionary::GetValues() const{
	decUnicodeStringList values;
	int i;
	
	for(i=0; i<pBucketCount; i++){
		sDictEntry *iterEntry = pBuckets[i];
		
		while(iterEntry){
			values.Add(iterEntry->value);
			iterEntry = iterEntry->next;
		}
	}
	
	return values;
}



bool decUnicodeStringDictionary::Equals(const decUnicodeStringDictionary &dict) const{
	decUnicodeString *string;
	int i;
	
	if(dict.pEntryCount != pEntryCount){
		return false;
	}
	
	for(i=0; i<pBucketCount; i++){
		sDictEntry *iterEntry = pBuckets[i];
		
		while(iterEntry){
			if(! dict.GetAt(iterEntry->key, &string) || *string != iterEntry->value){
				return false;
			}
			iterEntry = iterEntry->next;
		}
	}
	
	return true;
}



void decUnicodeStringDictionary::CheckLoad(){
	if((float)pEntryCount / (float)pBucketCount > 0.7){
		const int newBucketCount = pBucketCount + (pBucketCount >> 1); // +50%
		sDictEntry ** const newBuckets = new sDictEntry*[newBucketCount];
		int i;
		
		if(! newBuckets){
			DETHROW(deeInvalidParam);
		}
		for(i=0; i<newBucketCount; i++){
			newBuckets[i] = NULL;
		}
		
		for(i=0; i<pBucketCount; i++){
			sDictEntry *iterEntry = pBuckets[i];
			
			while(iterEntry){
				sDictEntry * const moveEntry = iterEntry;
				iterEntry = iterEntry->next;
				
				const int bucketIndex = (moveEntry->hash % newBucketCount);
				sDictEntry *iterEntry2 = newBuckets[bucketIndex];
				
				if(iterEntry2){
					while(iterEntry2->next){
						iterEntry2 = iterEntry2->next;
					}
					iterEntry2->next = moveEntry;
					
				}else{
					newBuckets[bucketIndex] = moveEntry;
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

bool decUnicodeStringDictionary::operator==(const decUnicodeStringDictionary &dict) const{
	return Equals(dict);
}

decUnicodeStringDictionary decUnicodeStringDictionary::operator+(const decUnicodeStringDictionary &dict) const{
	decUnicodeStringDictionary ndict(*this);
	int i;
	
	for(i=0; i<dict.pBucketCount; i++){
		sDictEntry *iterEntry = dict.pBuckets[i];
		
		while(iterEntry){
			ndict.SetAt(iterEntry->key, iterEntry->value);
			iterEntry = iterEntry->next;
		}
	}
	
	return ndict;
}

const decUnicodeString &decUnicodeStringDictionary::operator[](const decUnicodeString &key) const{
	return GetAt(key);
}



decUnicodeStringDictionary &decUnicodeStringDictionary::operator=(const decUnicodeStringDictionary &dict){
	RemoveAll();
	return *this += dict;
}

decUnicodeStringDictionary &decUnicodeStringDictionary::operator+=(const decUnicodeStringDictionary &dict){
	int i;
	
	for(i=0; i<dict.pBucketCount; i++){
		sDictEntry *iterEntry = dict.pBuckets[i];
		
		while(iterEntry){
			SetAt(iterEntry->key, iterEntry->value);
			iterEntry = iterEntry->next;
		}
	}
	
	return *this;
}

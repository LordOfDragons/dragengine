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

#include "decPointerDictionaryExt.h"
#include "decPointerList.h"
#include "../exceptions.h"



// Struct decPointerDictionaryExt::sDictEntry
///////////////////////////////////////////////

decPointerDictionaryExt::sDictEntry::sDictEntry() :
hash(0),
key(nullptr),
value(nullptr),
next(nullptr){
}

decPointerDictionaryExt::sDictEntry::sDictEntry(const decPointerDictionaryExt::sDictEntry &entry) :
hash(entry.hash),
key(entry.key),
value(entry.value),
next(nullptr){
}

decPointerDictionaryExt::sDictEntry::sDictEntry(unsigned int nhash, const void *nkey, void *nvalue) :
hash(nhash),
key(nkey),
value(nvalue),
next(nullptr){
}

decPointerDictionaryExt::sDictEntry::~sDictEntry(){
	value = nullptr;
	next = nullptr;
}



// Class decPointerDictionaryExt
//////////////////////////////

// Constructor, destructor
////////////////////////////

decPointerDictionaryExt::decPointerDictionaryExt(){
	pBuckets = NULL;
	pBucketCount = 8;
	pEntryCount = 0;
	
	pBuckets = new sDictEntry*[pBucketCount];
	
	int i;
	for(i=0; i<pBucketCount; i++){
		pBuckets[i] = NULL;
	}
}

decPointerDictionaryExt::decPointerDictionaryExt(int bucketCount){
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

decPointerDictionaryExt::decPointerDictionaryExt(const decPointerDictionaryExt &dict){
	pBuckets = nullptr;
	pBucketCount = dict.pBucketCount;
	pEntryCount = dict.pEntryCount;
	
	pBuckets = new sDictEntry*[pBucketCount];
	
	int i;
	for(i=0; i<pBucketCount; i++){
		pBuckets[i] = nullptr;
	}
	
	for(i=0; i<pBucketCount; i++){
		sDictEntry *iterEntry = dict.pBuckets[i];
		sDictEntry *lastEntry = nullptr;
		
		while(iterEntry){
			sDictEntry * const newEntry = new sDictEntry(*iterEntry);
			
			if(lastEntry){
				lastEntry->next = newEntry;
				
			}else{
				pBuckets[i] = newEntry;
			}
			lastEntry = newEntry;
			
			iterEntry = iterEntry->next;
		}
	}
}

decPointerDictionaryExt::~decPointerDictionaryExt(){
	RemoveAll();
	
	if(pBuckets){
		delete [] pBuckets;
		pBuckets = NULL;
	}
	
	pBucketCount = 0;
}



// Management
///////////////

bool decPointerDictionaryExt::Has(const void *key, unsigned int hash) const{
	if(!key){
		DETHROW(deeNullPointer);
	}
	
	sDictEntry *iterEntry = pBuckets[hash % pBucketCount];
	
	while(iterEntry){
		if(iterEntry->key == key){
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void *decPointerDictionaryExt::GetAt(const void *key, unsigned int hash) const{
	void *pointer;
	
	if(!GetAt(key, hash, &pointer)){
		DETHROW(deeInvalidParam);
	}
	
	return pointer;
}

bool decPointerDictionaryExt::GetAt(const void *key, unsigned int hash, void **pointer) const{
	if(!key || !pointer){
		DETHROW(deeNullPointer);
	}
	
	sDictEntry *iterEntry = pBuckets[hash % pBucketCount];
	
	while(iterEntry){
		if(iterEntry->key == key){
			*pointer = iterEntry->value;
			return true;
		}
		iterEntry = iterEntry->next;
	}
	
	return false;
}

void decPointerDictionaryExt::SetAt(const void *key, unsigned int hash, void *value){
	if(!key){
		DETHROW(deeNullPointer);
	}
	
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[bucketIndex];
	sDictEntry *lastEntry = NULL;
	
	while(iterEntry){
		if(iterEntry->key == key){
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

void decPointerDictionaryExt::Remove(const void *key, unsigned int hash){
	if(!key){
		DETHROW(deeNullPointer);
	}
	
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[bucketIndex];
	sDictEntry *lastEntry = NULL;
	
	while(iterEntry){
		if(iterEntry->key == key){
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

void decPointerDictionaryExt::RemoveIfPresent(const void *key, unsigned int hash){
	if(!key){
		DETHROW(deeNullPointer);
	}
	
	const int bucketIndex = hash % pBucketCount;
	
	sDictEntry *iterEntry = pBuckets[bucketIndex];
	sDictEntry *lastEntry = NULL;
	
	while(iterEntry){
		if(iterEntry->key == key){
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

void decPointerDictionaryExt::RemoveAll(){
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



decPointerList decPointerDictionaryExt::GetKeys() const{
	decPointerList keys;
	int i;
	
	for(i=0; i<pBucketCount; i++){
		sDictEntry *iterEntry = pBuckets[i];
		
		while(iterEntry){
			keys.Add((void*)iterEntry->key); // this is bad, I know, but necessary
			iterEntry = iterEntry->next;
		}
	}
	
	return keys;
}

decPointerList decPointerDictionaryExt::GetValues() const{
	decPointerList values;
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



bool decPointerDictionaryExt::Equals(const decPointerDictionaryExt &dict) const{
	void *object;
	int i;
	
	if(dict.pEntryCount != pEntryCount){
		return false;
	}
	
	for(i=0; i<pBucketCount; i++){
		sDictEntry *iterEntry = pBuckets[i];
		
		while(iterEntry){
			if(!dict.GetAt(iterEntry->key, iterEntry->hash, &object) || object != iterEntry->value){
				return false;
			}
			iterEntry = iterEntry->next;
		}
	}
	
	return true;
}



void decPointerDictionaryExt::CheckLoad(){
	if((float)pEntryCount / (float)pBucketCount > 0.7){
		const int newBucketCount = pBucketCount + (pBucketCount >> 1); // +50%
		sDictEntry ** const newBuckets = new sDictEntry*[newBucketCount];
		int i;
		
		if(!newBuckets){
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

bool decPointerDictionaryExt::operator==(const decPointerDictionaryExt &dict) const{
	return Equals(dict);
}

decPointerDictionaryExt decPointerDictionaryExt::operator+(const decPointerDictionaryExt &dict) const{
	decPointerDictionaryExt ndict(*this);
	int i;
	
	for(i=0; i<dict.pBucketCount; i++){
		sDictEntry *iterEntry = dict.pBuckets[i];
		
		while(iterEntry){
			ndict.SetAt(iterEntry->key, iterEntry->hash, iterEntry->value);
			iterEntry = iterEntry->next;
		}
	}
	
	return ndict;
}



decPointerDictionaryExt &decPointerDictionaryExt::operator=(const decPointerDictionaryExt &dict){
	RemoveAll();
	return *this += dict;
}

decPointerDictionaryExt &decPointerDictionaryExt::operator+=(const decPointerDictionaryExt &dict){
	int i;
	
	for(i=0; i<dict.pBucketCount; i++){
		sDictEntry *iterEntry = dict.pBuckets[i];
		
		while(iterEntry){
			SetAt(iterEntry->key, iterEntry->hash, iterEntry->value);
			iterEntry = iterEntry->next;
		}
	}
	
	return *this;
}

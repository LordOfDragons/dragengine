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

#include "decUniqueID.h"
#include "../math/decMath.h"
#include "../exceptions.h"



// Class decUniqueID
//////////////////////

// Constructor, destructor
////////////////////////////

decUniqueID::decUniqueID() :
pBytes(new unsigned char[1]),
pByteCount(1){
	pBytes[0] = 0;
}

decUniqueID::decUniqueID(int value) :
pBytes(NULL),
pByteCount(0)
{
	if(value < 0){
		DETHROW(deeInvalidParam);
	}
	
	// determine the number of number of bytes required to store value
	int requiredByteCount = 1;
	int testValue = value >> 8;
	while(testValue > 0){
		requiredByteCount++;
		testValue >>= 8;
	}
	
	// store value
	pBytes = new unsigned char[requiredByteCount];
	if(value > 0){
		while(value > 0){
			pBytes[pByteCount++] = (unsigned char)(value & 0xff);
			value >>= 8;
		}
		
	}else{
		pBytes[0] = 0;
		pByteCount = 1;
	}
}

decUniqueID::decUniqueID(const char *string) :
pBytes(NULL),
pByteCount(0)
{
	if(!string || !string[0]){
		DETHROW(deeInvalidParam);
	}
	
	const int nibbleCount = (int)strlen(string);
	
	pByteCount = (nibbleCount / 2) + (nibbleCount % 2);
	pBytes = new unsigned char[pByteCount];
	memset(pBytes, 0, pByteCount);
	
	int shift = (nibbleCount % 2) == 0 ? 4 : 0;
	int index = pByteCount - 1;
	int i;
	
	for(i=0; i<nibbleCount; i++){
		if(string[i] >= '0' && string[i] <= '9'){
			pBytes[index] |= (string[i] - '0') << shift;
			
		}else if(string[i] >= 'a' && string[i] <= 'z'){
			pBytes[index] |= (string[i] - 'a' + 10) << shift;
			
		}else if(string[i] >= 'A' && string[i] <= 'Z'){
			pBytes[index] |= (string[i] - 'A' + 10) << shift;
			
		}else{
			DETHROW(deeInvalidParam);
		}
		
		if(shift == 0){
			index--;
			shift = 4;
			
		}else{
			shift = 0;
		}
	}
}

decUniqueID::decUniqueID(const decUniqueID &id) :
pBytes(new unsigned char[id.pByteCount]),
pByteCount(id.pByteCount){
	memcpy(pBytes, id.pBytes, pByteCount);
}

decUniqueID::~decUniqueID(){
	if(pBytes){
		delete [] pBytes;
	}
}



// Management
///////////////

void decUniqueID::SetBitCount(int bitCount){
	if(bitCount < 8 || bitCount % 8 != 0){
		DETHROW(deeInvalidParam);
	}
	
	const int byteCount = bitCount >> 3;
	if(byteCount == pByteCount){
		return;
	}
	
	unsigned char * const bytes = new unsigned char[byteCount];
	if(pBytes){
		delete [] pBytes;
	}
	pBytes = bytes;
	pByteCount = byteCount;
	
	memset(pBytes, 0, pByteCount);
}

int decUniqueID::GetByteAt(int position) const{
	if(position < 0 || position >= pByteCount){
		DETHROW(deeInvalidParam);
	}
	return pBytes[position];
}

void decUniqueID::SetByteAt(int position, int value){
	if(position < 0 || position >= pByteCount){
		DETHROW(deeInvalidParam);
	}
	if(value < 0 || value > 255){
		DETHROW(deeInvalidParam);
	}
	pBytes[position] = (unsigned char)value;
}

void decUniqueID::Increment(){
	int i;
	
	for(i=0; i<pByteCount; i++){
		// increase the value of the byte by one. this wraps around going past 255
		pBytes[i]++;
		
		// if the value wrapped around the new value is 0 now. If this is not the case break out
		if(pBytes[i] > 0){
			break;
		}
		
		// otherwise we have to increment the next byte
	}
	
	// if we went past the last byte this means that the new value can
	// not be hold anymore using the current bit count.
	if(i == pByteCount){
		// increase the bit count. this sets all bytes to 0
		SetBitCount((pByteCount + 1) << 3);
		
		// set the top most byte to 1
		pBytes[pByteCount - 1] = 1;
	}
}

void decUniqueID::IncrementBy(int amount){
	int takeOver = 0;
	int i;
	
	for(i=0; i<pByteCount; i++){
		takeOver += (int)pBytes[i] + (amount & 0xff);
		pBytes[i] = (unsigned char)(takeOver & 0xff);
		takeOver >>= 8;
		
		amount >>= 8;
		if(amount == 0 && takeOver == 0){
			return;
		}
		
		if(i == pByteCount - 1){
			unsigned char * const bytes = new unsigned char[pByteCount + 1];
			memcpy(bytes, pBytes, pByteCount);
			bytes[pByteCount] = 0;
			delete [] pBytes;
			pBytes = bytes;
			pByteCount++;
		}
	}
}

void decUniqueID::IncrementBy(const decUniqueID &id){
	int takeOver = 0;
	int i;
	
	for(i=0; i<pByteCount; i++){
		takeOver += (int)pBytes[i];
		if(i < id.pByteCount){
			takeOver += id.pBytes[i];
		}
		pBytes[i] = (unsigned char)(takeOver & 0xff);
		takeOver >>= 8;
		
		if(i >= id.pByteCount - 1 && takeOver == 0){
			return;
		}
		
		if(i == pByteCount - 1){
			unsigned char * const bytes = new unsigned char[pByteCount + 1];
			memcpy(bytes, pBytes, pByteCount);
			bytes[pByteCount] = 0;
			delete [] pBytes;
			pBytes = bytes;
			pByteCount++;
		}
	}
}

void decUniqueID::Decrement(){
	int i;
	
	for(i=0; i<pByteCount; i++){
		if(pBytes[i] > 0){
			break;
		}
	}
	if(i == pByteCount){
		DETHROW(deeInvalidParam);
	}
	
	for(i=0; i<pByteCount; i++){
		if(pBytes[i] > 0){
			pBytes[i]--;
			break;
		}
		
		pBytes[i] = 255;
	}
}

void decUniqueID::DecrementBy(int amount){
	if(amount < 0){
		DETHROW(deeInvalidParam);
	}
	
	DecrementBy(decUniqueID(amount));
}

void decUniqueID::DecrementBy(const decUniqueID &id){
	if(id > *this){
		DETHROW(deeInvalidParam);
	}
	
	int i;
	int takeOver = 0;
	
	for(i=0; i<pByteCount; i++){
		if(i < id.pByteCount){
			takeOver += id.pBytes[i];
		}
		
		if(takeOver > pBytes[i]){
			pBytes[i] += (unsigned char)(256 - takeOver);
			takeOver = 1;
			
		}else{
			pBytes[i] -= (unsigned char)takeOver;
			takeOver = 0;
		}
	}
}

void decUniqueID::Reset(){
	SetBitCount(8);
}

decString decUniqueID::ToHexString() const{
	const int nibbleCount = pByteCount * 2;
	int index = pByteCount - 1;
	int shift = 4;
	int i;
	
	decString string;
	string.Set(' ', nibbleCount);
	
	for(i=0; i<nibbleCount; i++){
		const int value = (pBytes[index] >> shift) & 0xf;
		
		if(value < 10){
			string[i] = (char)('0' + value);
			
		}else{
			string[i] = (char)('a' + (value - 10));
		}
		
		if(shift == 0){
			index--;
			shift = 4;
			
		}else{
			shift = 0;
		}
	}
	
	for(i=0; i<nibbleCount; i++){
		if(string[i] != '0'){
			return string.GetMiddle(i);
		}
	}
	
	return "0";
}

bool decUniqueID::IsZero() const{
	int i;
	for(i=0; i<pByteCount; i++){
		if(pBytes[i]){
			return false;
		}
	}
	return true;
}

unsigned int decUniqueID::Hash() const{
	const int count = decMath::min(pByteCount, 4);
	unsigned int hash = 0;
	int i;
	
	for(i=0; i<count; i++){
		hash |= pBytes[i] << (8 * i);
	}
	
	return hash;
}


// Operators
//////////////

decUniqueID &decUniqueID::operator=(const decUniqueID &id){
	int b;
	
	SetBitCount(id.pByteCount << 3);
	
	for(b=0; b<pByteCount; b++){
		pBytes[b] = id.pBytes[b];
	}
	
	return *this;
}

bool decUniqueID::operator==(const decUniqueID &id) const{
	int b, value1, value2, byteCount;
	
	// determine the largest of the two byte counts
	if(pByteCount > id.pByteCount){
		byteCount = pByteCount;
		
	}else{
		byteCount = id.pByteCount;
	}
	
	// compare the values
	for(b=0; b<byteCount; b++){
		// obtain first value or 0 if outside range
		if(b < pByteCount){
			value1 = pBytes[b];
			
		}else{
			value1 = 0;
		}
		
		// obtain second value or 0 if outside range
		if(b < id.pByteCount){
			value2 = id.pBytes[b];
			
		}else{
			value2 = 0;
		}
		
		// if the values are different the ids are different too
		if(value1 != value2) return false;
	}
	
	// ids are the same
	return true;
}

bool decUniqueID::operator!=(const decUniqueID &id) const{
	return !(*this == id);
}

bool decUniqueID::operator<(const decUniqueID& id) const
{
	const int byteCount = decMath::max(pByteCount, id.pByteCount);
	int i;
	
	for(i=byteCount-1; i>=0; i--){
		const int value1 = i < pByteCount ? pBytes[i] : 0;
		const int value2 = i < id.pByteCount ? id.pBytes[i] : 0;
		if(value1 != value2){
			return value1 < value2;
		}
	}
	
	return false;
}

bool decUniqueID::operator<=(const decUniqueID& id) const
{
	const int byteCount = decMath::max(pByteCount, id.pByteCount);
	int i;
	
	for(i=byteCount-1; i>=0; i--){
		const int value1 = i < pByteCount ? pBytes[i] : 0;
		const int value2 = i < id.pByteCount ? id.pBytes[i] : 0;
		if(value1 != value2){
			return value1 < value2;
		}
	}
	
	return true;
}

bool decUniqueID::operator>(const decUniqueID& id) const
{
	const int byteCount = decMath::max(pByteCount, id.pByteCount);
	int i;
	
	for(i=byteCount-1; i>=0; i--){
		const int value1 = i < pByteCount ? pBytes[i] : 0;
		const int value2 = i < id.pByteCount ? id.pBytes[i] : 0;
		if(value1 != value2){
			return value1 > value2;
		}
	}
	
	return false;
}

bool decUniqueID::operator>=(const decUniqueID& id) const
{
	const int byteCount = decMath::max(pByteCount, id.pByteCount);
	int i;
	
	for(i=byteCount-1; i>=0; i--){
		const int value1 = i < pByteCount ? pBytes[i] : 0;
		const int value2 = i < id.pByteCount ? id.pBytes[i] : 0;
		if(value1 != value2){
			return value1 > value2;
		}
	}
	
	return true;
}

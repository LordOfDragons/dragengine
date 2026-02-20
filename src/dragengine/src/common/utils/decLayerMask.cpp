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

#include "decLayerMask.h"

#include "../exceptions.h"
#include "../string/decString.h"
#include "../file/decBaseFileReader.h"
#include "../file/decBaseFileWriter.h"



// Class decLayerMask
///////////////////////

// Constructor, destructor
////////////////////////////

decLayerMask::decLayerMask() :
pMask(0){
}

decLayerMask::decLayerMask(const decLayerMask &layerMask) :
pMask(layerMask.pMask){
}

decLayerMask::~decLayerMask(){
}



// Management
///////////////

void decLayerMask::ClearMask(){
	pMask = 0;
}

void decLayerMask::FillMask(){
	pMask = ~(uint64_t)0;
}

void decLayerMask::SetBit(int bit){
	if(bit < 0 || bit > 63){
		DETHROW(deeInvalidParam);
	}
	
	pMask |= (uint64_t)1 << bit;
}

void decLayerMask::ClearBit(int bit){
	if(bit < 0 || bit > 63){
		DETHROW(deeInvalidParam);
	}
	
	pMask &= ~((uint64_t)1 << bit);
}

bool decLayerMask::IsBitSet(int bit) const{
	if(bit < 0 || bit > 63){
		DETHROW(deeInvalidParam);
	}
	
	return ((pMask >> bit) & 1) == 1;
}

bool decLayerMask::IsBitCleared(int bit) const{
	if(bit < 0 || bit > 63){
		DETHROW(deeInvalidParam);
	}
	
	return ((pMask >> bit) & 1) == 0;
}

bool decLayerMask::IsEmpty() const{
	return pMask == 0;
}

bool decLayerMask::IsNotEmpty() const{
	return pMask != 0;
}

bool decLayerMask::IsFull() const{
	return ~pMask == 0;
}

bool decLayerMask::Matches(const decLayerMask &layerMask) const{
	return (pMask & layerMask.pMask) != 0;
}

bool decLayerMask::MatchesNot(const decLayerMask &layerMask) const {
	return (pMask & layerMask.pMask) == 0;
}



decString decLayerMask::ToBitString() const{
	decString s;
	int i;
	
	s.Set('0', 64);
	
	for(i=0; i<64; i++){
		if(((pMask >> i) & 1) == 1){
			s.SetAt(i, '1');
		}
	}
	
	return s;
}

void decLayerMask::SetFromBitString(const char *string){
	if(!string){
		DETHROW(deeInvalidParam);
	}
	
	const int len = (int)strlen(string);
	int i;
	
	pMask = 0;
	
	for(i=0; i<64; i++){
		if(i < len && string[len] == '1'){
			pMask |= (uint64_t)1 << i;
		}
	}
}

decString decLayerMask::ToHexString() const{
	decString s;
	int i;
	
	s.Set('0', 16);
	
	for(i=0; i<64; i+=4){
		/*
		int value = 0;
		
		if(((pMask >> i) & 1) == 1){
			value |= 8;
		}
		if(((pMask >> (i + 1)) & 1) == 1){
			value |= 4;
		}
		if(((pMask >> (i + 2)) & 1) == 1){
			value |= 2;
		}
		if(((pMask >> (i + 3)) & 1) == 1){
			value |= 1;
		}
		*/
		const int value = (pMask >> i) & 0xf;
		
		if(value < 10){
			s.SetAt(i >> 2, '0' + value);
			
		}else{
			s.SetAt(i >> 2, 'a' + (value - 10));
		}
	}
	
	return s;
}

void decLayerMask::SetFromHexString(const char *string){
	if(!string){
		DETHROW(deeInvalidParam);
	}
	
	const int len = (int)strlen(string);
	int i;
	
	pMask = 0;
	
	for(i=0; i<16; i++){
		if(i < len){
			int value = 0;
			
			if(string[i] >= '0' && string[i] <= '9'){
				value = string[i] - '0';
				
			}else if(string[i] >= 'a' && string[i] <= 'z'){
				value = string[i] - 'a' + 10;
				
			}else if(string[i] >= 'A' && string[i] <= 'A'){
				value = string[i] - 'A' + 10;
			}
			
			/*
			if((value & 8) == 8){
				pMask |= (uint64_t)1 << (i * 4);
			}
			if((value & 4) == 4){
				pMask |= (uint64_t)1 << (i * 4 + 1);
			}
			if((value & 2) == 2){
				pMask |= (uint64_t)1 << (i * 4 + 2);
			}
			if((value & 1) == 1){
				pMask |= (uint64_t)1 << (i * 4 + 3);
			}*/
			pMask |= (uint64_t)value << (i * 4);
		}
	}
}

decLayerMask decLayerMask::ReadFromFile(decBaseFileReader &reader){
	decLayerMask layerMask;
	layerMask.pMask = reader.ReadULong();
	return layerMask;
}

void decLayerMask::WriteToFile(decBaseFileWriter& writer) const{
	writer.WriteULong(pMask);
}

void decLayerMask::SetMask(uint64_t mask){
	pMask = mask;
}



// Operators
//////////////

bool decLayerMask::operator==(const decLayerMask &layerMask) const{
	return pMask == layerMask.pMask;
}

bool decLayerMask::operator!=(const decLayerMask &layerMask) const{
	return pMask != layerMask.pMask;
}

decLayerMask &decLayerMask::operator=(const decLayerMask &layerMask){
	pMask = layerMask.pMask;
	return *this;
}

decLayerMask &decLayerMask::operator|=(const decLayerMask &layerMask){
	pMask |= layerMask.pMask;
	return *this;
}

decLayerMask &decLayerMask::operator&=(const decLayerMask &layerMask){
	pMask &= layerMask.pMask;
	return *this;
}

decLayerMask &decLayerMask::operator^=(const decLayerMask &layerMask){
	pMask ^= layerMask.pMask;
	return *this;
}

decLayerMask decLayerMask::operator|(const decLayerMask &layerMask) const{
	decLayerMask newMask;
	newMask.pMask = pMask | layerMask.pMask;
	return newMask;
}

decLayerMask decLayerMask::operator&(const decLayerMask &layerMask) const{
	decLayerMask newMask;
	newMask.pMask = pMask & layerMask.pMask;
	return newMask;
}

decLayerMask decLayerMask::operator^(const decLayerMask &layerMask) const{
	decLayerMask newMask;
	newMask.pMask = pMask ^ layerMask.pMask;
	return newMask;
}

decLayerMask decLayerMask::operator~() const{
	decLayerMask newMask;
	newMask.pMask = ~pMask;
	return newMask;
}

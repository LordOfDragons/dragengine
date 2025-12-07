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

#include "decUTF8Decoder.h"
#include "../../exceptions.h"



// class decUTF8Decoder
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

decUTF8Decoder::decUTF8Decoder(){
	pString = "";
	pLength = 0;
	pPosition = 0;
}

decUTF8Decoder::~decUTF8Decoder(){
}



// Accessors and Mutators
///////////////////////////

bool decUTF8Decoder::HasReachedEnd() const{
	return pPosition == pLength;
}

void decUTF8Decoder::SetString(const char *string){
	if(!string) DETHROW(deeInvalidParam);
	pString = string;
	pLength = strlen(string);
	pPosition = 0;
}

void decUTF8Decoder::SetPosition(int position){
	if(position < 0 || position > pLength) DETHROW(deeInvalidParam);
	pPosition = position;
}

int decUTF8Decoder::DecodeNextCharacter(){
	int followBytes;
	int character;
	
	// check for end of pString
	if(pPosition == pLength) return -1;
	
	// decode start byte
	if(pString[pPosition] & 0x80){ // 1xxxxxxx
		
		if(pString[pPosition] & 0x40){ // 11xxxxxx
			
			if(pString[pPosition] & 0x20){ // 111xxxxx
				
				if(pString[pPosition] & 0x10){ // 1111xxxx
					
					if(pString[pPosition] & 0x08){ // 11111xxx not allowed
						pPosition++;
						return -1;
						
					}else{ // 11110xxx
						character = pString[pPosition++] & 0x07;
						followBytes = 3;
					}
					
				}else{ // 1110xxxx
					character = pString[pPosition++] & 0x0f;
					followBytes = 2;
				}
				
			}else{ // 110xxxxx
				character = pString[pPosition++] & 0x1f;
				followBytes = 1;
			}
			
		}else{ // 10xxxxxx is not allowed
			pPosition++;
			return -1;
		}
		
	}else{ // 0xxxxxxx
		return pString[pPosition++];
	}
	
	// decode follow bytes
	while(followBytes > 0){
		
		if(pPosition == pLength) return -1;
		
		if(pString[pPosition] & 0x80){ // 1xxxxxxx
			
			if(pString[pPosition] & 0x40){ // 11xxxxxx is not allowed
				//pPosition++; <= nah, this is a new start byte so don't skip it!
				return -1;
				
			}else{ // 10xxxxxx
				character = (character << 6) | (pString[pPosition++] & 0x3f);
				followBytes--;
			}
			
		}else{ // 0xxxxxxx is not allowed
			pPosition++;
			return -1;
		}
		
	}
	
	// return the character
	return character;
}

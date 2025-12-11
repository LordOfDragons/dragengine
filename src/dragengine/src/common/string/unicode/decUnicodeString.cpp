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

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "decUnicodeString.h"
#include "decUTF8Decoder.h"
#include "decUnicodeStringList.h"
#include "../decString.h"
#include "../../exceptions.h"
#include "../../../dragengine_configuration.h"



// Helpers
////////////

static int pToLower(int character){
	if(character < 0){
		DETHROW(deeInvalidParam);
	}
	if(character > 255){
		return character; // tolower can not handle values larger 255
	}
	return tolower(character);
}

static int pToUpper(int character){
	if(character < 0){
		DETHROW(deeInvalidParam);
	}
	if(character > 255){
		return character; // toupper can not handle values larger 255
	}
	return toupper(character);
}



// Class decUnicodeString
///////////////////////////

// Constructors and Destructors
/////////////////////////////////

decUnicodeString::decUnicodeString(){
	pString = NULL;
	pLength = 0;
	pSize = 1;
}

decUnicodeString::decUnicodeString(const decUnicodeString &string){
	pString = NULL;
	pLength = 0;
	
	if(string.pLength > 0){
		pCreateString(string.pSize, string.pLength);
		memcpy(pString, string.pString, string.pLength * string.pSize);
	}
}

decUnicodeString::decUnicodeString(const decUnicodeString &string1, const decUnicodeString &string2){
	pString = NULL;
	pLength = 0;
	
	if(string1.pLength + string2.pLength > 0){
		int p = 0;
		int i;
		
		pCreateString(string1.pSize > string2.pSize ? string1.pSize : string2.pSize, string1.pLength + string2.pLength);
		
		for(i=0; i<string1.pLength; i++){
			pSetAt(p++, string1.pGetAt(i));
		}
		for(i=0; i<string2.pLength; i++){
			pSetAt(p++, string2.pGetAt(i));
		}
	}
}

decUnicodeString::decUnicodeString(int character){
	pString = NULL;
	pCreateString(pCharSize(character), 1);
	pSetAt(0, character);
}

decUnicodeString decUnicodeString::NewFromUTF8(const char *string){
	decUnicodeString ustring;
	ustring.SetFromUTF8(string);
	return ustring;
}

decUnicodeString::~decUnicodeString(){
	pFreeString();
}



// Accessors and Mutators
///////////////////////////

bool decUnicodeString::IsEmpty() const{
	return pLength == 0;
}

void decUnicodeString::Empty(){
	pFreeString();
}

int decUnicodeString::GetAt(int position) const{
	if(position < 0 || position >= pLength){
		DETHROW(deeInvalidParam);
	}
	return pGetAt(position);
}

void decUnicodeString::SetAt(int position, int character){
	if(position < 0 || position >= pLength){
		DETHROW(deeInvalidParam);
	}
	pGrowString(pCharSize(character), pLength);
	pSetAt(position, character);
}



void decUnicodeString::Set(const decUnicodeString &string){
	if(string.pLength > 0){
		int i;
		
		pCreateString(string.pSize, string.pLength);
		
		for(i=0; i<string.pLength; i++){
			pSetAt(i, string.pGetAt(i));
		}
		
	}else{
		pFreeString();
	}
}

void decUnicodeString::Set(int character, int count){
	if(count > 0){
		int i;
		
		pCreateString(pCharSize(character), count);
		
		for(i=0; i<count; i++){
			pSetAt(i, character);
		}
		
	}else{
		pFreeString();
	}
}

void decUnicodeString::SetValue(char value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(unsigned char value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(short value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(unsigned short value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(int value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(unsigned int value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(float value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetValue(double value){
	decString temp;
	temp.SetValue(value);
	const int length = temp.GetLength();
	pCreateString(1, length);
	#ifdef OS_W32_VS
		strcpy_s(reinterpret_cast<char*>(pString), length + 1, temp.GetString());
	#else
		strcpy(reinterpret_cast<char*>(pString), temp.GetString());
	#endif
}

void decUnicodeString::SetFromUTF8(const char *string){
	if(!string){
		DETHROW(deeInvalidParam);
	}
	
	// determine the maximum size required to store this string
	decUTF8Decoder decoder;
	decoder.SetString(string);
	int usize = 1;
	int ulen = 0;
	
	while(!decoder.HasReachedEnd()){
		const int character = decoder.DecodeNextCharacter();
		
		if(character != -1){
			const int csize = pCharSize(character);
			
			ulen++;
			if(csize > usize){
				usize = csize;
			}
		}
	}
	
	// copy string
	pCreateString(usize, ulen);
	decoder.SetPosition(0);
	int i = 0;
	
	while(i < ulen){
		const int character = decoder.DecodeNextCharacter();
		
		if(character != -1){
			pSetAt(i++, character);
		}
	}
}

void decUnicodeString::Format(const char *format, ...){
	va_list list;
	va_start(list, format);
	FormatUsing(format, list);
	va_end(list);
}

void decUnicodeString::FormatUsing(const char *format, va_list args){
	decString temp;
	temp.FormatUsing(format, args);
	SetFromUTF8(temp);
}



void decUnicodeString::Append(const decUnicodeString &string){
	if(string.pLength > 0){
		int pos = pLength;
		int i;
		
		pGrowString(string.pSize, pLength + string.pLength);
		
		for(i=0; i<string.pLength; i++){
			pSetAt(pos++, string.pGetAt(i));
		}
	}
}

void decUnicodeString::AppendCharacter(char character){
	pGrowString(1, pLength + 1);
	pSetAt(pLength - 1, character);
}

void decUnicodeString::AppendCharacter(unsigned char character){
	pGrowString(1, pLength + 1);
	pSetAt(pLength - 1, character);
}

void decUnicodeString::AppendCharacter(int character){
	pGrowString(pCharSize(character), pLength + 1);
	pSetAt(pLength - 1, character);
}

void decUnicodeString::AppendValue(char value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(unsigned char value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(short value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(unsigned short value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(int value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(unsigned int value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(float value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendValue(double value){
	decUnicodeString temp;
	temp.SetValue(value);
	Append(temp);
}

void decUnicodeString::AppendFromUTF8(const char *string){
	if(!string){
		DETHROW(deeInvalidParam);
	}
	
	// determine the maximum size required to store this string
	decUTF8Decoder decoder;
	decoder.SetString(string);
	int usize = 1;
	int ulen = 0;
	
	while(!decoder.HasReachedEnd()){
		const int character = decoder.DecodeNextCharacter();
		
		if(character != -1){
			const int csize = pCharSize(character);
			
			ulen++;
			if(csize > usize){
				usize = csize;
			}
		}
	}
	
	// copy string
	const int oldLength = pLength;
	
	pGrowString(usize, pLength + ulen);
	decoder.SetPosition(0);
	int i = oldLength;
	
	while(i < pLength){
		const int character = decoder.DecodeNextCharacter();
		
		if(character != -1){
			pSetAt(i++, character);
		}
	}
}

void decUnicodeString::AppendFormat(const char *format, ...){
	va_list list;
	va_start(list, format);
	AppendFormatUsing(format, list);
	va_end(list);
}

void decUnicodeString::AppendFormatUsing(const char *format, va_list args){
	decUnicodeString temp;
	temp.FormatUsing(format, args);
	Append(temp);
}



int decUnicodeString::Find(int character) const{
	return Find(character, 0, pLength);
}

int decUnicodeString::Find(int character, int start) const{
	return Find(character, start, pLength);
}

int decUnicodeString::Find(int character, int start, int end) const{
	int i;
	
	if(start < 0){
		start = pLength + start;
	}
	if(end < 0){
		end = pLength + end;
	}
	
	if(start < 0){
		start = 0;
	}
	if(end > pLength){
		end = pLength;
	}
	
	for(i=start; i<end; i++){
		if(pGetAt(i) == character){
			return i;
		}
	}
	
	return -1;
}

int decUnicodeString::Find(const decUnicodeString &characters) const{
	return Find(characters, 0, pLength);
}

int decUnicodeString::Find(const decUnicodeString &characters, int start) const{
	return Find(characters, start, pLength);
}

int decUnicodeString::Find(const decUnicodeString &characters, int start, int end) const{
	int i, found, foundBest = -1;
	
	for(i=0; i<characters.pLength; i++){
		found = Find(characters.pGetAt(i), start, end);
		
		if(found != -1 && (foundBest == -1 || found < foundBest)){
			foundBest = found;
		}
	}
	
	return foundBest;
}

int decUnicodeString::FindReverse(int character) const{
	return FindReverse(character, 0, pLength);
}

int decUnicodeString::FindReverse(int character, int start) const{
	return FindReverse(character, start, pLength);
}

int decUnicodeString::FindReverse(int character, int start, int end) const{
	int i;
	
	if(start < 0){
		start = pLength + start;
	}
	if(end < 0){
		end = pLength + end;
	}
	
	if(start < 0){
		start = 0;
	}
	if(end > pLength){
		end = pLength;
	}
	
	for(i=end-1; i>=start; i--){
		if(pGetAt(i) == character){
			return i;
		}
	}
	
	return -1;
}

int decUnicodeString::FindReverse(const decUnicodeString &characters) const{
	return FindReverse(characters, 0, pLength);
}

int decUnicodeString::FindReverse(const decUnicodeString &characters, int start) const{
	return FindReverse(characters, start, pLength);
}

int decUnicodeString::FindReverse(const decUnicodeString &characters, int start, int end) const{
	int i, found, foundBest = -1;
	
	for(i=0; i<characters.pLength; i++){
		found = FindReverse(characters.pGetAt(i), start, end);
		
		if(found != -1 && found > foundBest){
			foundBest = found;
		}
	}
	
	return foundBest;
}

int decUnicodeString::FindString(const decUnicodeString &string) const{
	return FindString(string, 0, pLength);
}

int decUnicodeString::FindString(const decUnicodeString &string, int start) const{
	return FindString(string, start, pLength);
}

int decUnicodeString::FindString(const decUnicodeString &string, int start, int end) const{
	const int len = pLength - string.pLength;
	int i, j;
	
	if(len < 0){
		return -1;
	}
	
	if(start < 0){
		start = pLength + start;
	}
	if(end < 0){
		end = pLength + end;
	}
	
	if(start < 0){
		start = 0;
	}
	if(end > len){
		end = len;
	}
	
	for(i=start; i<end; i++){
		for(j=0; j<string.pLength; j++){
			if(i + j >= end || pGetAt(i + j) != string.pGetAt(j)){
				break;
			}
		}
		if(j == string.pLength){
			return i;
		}
	}
	
	return -1;
}

int decUnicodeString::FindStringReverse(const decUnicodeString &string) const{
	return FindStringReverse(string, 0, pLength);
}

int decUnicodeString::FindStringReverse(const decUnicodeString &string, int start) const{
	return FindStringReverse(string, start, pLength);
}

int decUnicodeString::FindStringReverse(const decUnicodeString &string, int start, int end) const{
	const int len = pLength - string.pLength;
	int i, j;
	
	if(len < 0){
		return -1;
	}
	
	if(start < 0){
		start = pLength + start;
	}
	if(end < 0){
		end = pLength + end;
	}
	
	if(start < 0){
		start = 0;
	}
	if(end > len){
		end = len;
	}
	
	for(i=end-1; i>=start; i--){
		for(j=0; j<string.pLength; j++){
			if(i + j >= end || pGetAt(i + j) != string.pGetAt(j)){
				break;
			}
		}
		if(j == string.pLength){
			return i;
		}
	}
	
	return -1;
}



decUnicodeString decUnicodeString::GetLeft(int count) const{
	if(count < 0){
		count = 0;
	}
	
	return GetMiddle(0, count);
}

decUnicodeString decUnicodeString::GetRight(int count) const{
	if(count < 0){
		count = 0;
	}
	if(count > pLength){
		count = pLength;
	}
	
	return GetMiddle(pLength - count, pLength);
}

decUnicodeString decUnicodeString::GetMiddle(int start) const{
	return GetMiddle(start, pLength);
}

decUnicodeString decUnicodeString::GetMiddle(int start, int end) const{
	if(start < 0){
		start = pLength + start;
	}
	if(end < 0){
		end = pLength + end;
	}
	
	if(start < 0){
		start = 0;
	}
	if(end > pLength){
		end = pLength;
	}
	
	decUnicodeString string;
	
	if(start < end){
		const int count = end - start;
		int i;
		
		string.pCreateString(pSize, count);
		for(i=0; i<count; i++){
			string.pSetAt(i, pGetAt(start + i));
		}
	}
	
	return string;
}



void decUnicodeString::Reverse(){
	int start, end;
	
	for(start=0, end=GetLength()-1; start<end; start++, end--){
		const int swap = pGetAt(start);
		pSetAt(start, pGetAt(end));
		pSetAt(end, swap);
	}
}

decUnicodeString decUnicodeString::GetReversed() const{
	decUnicodeString string;
	
	if(pLength > 0){
		int from, to;
		
		string.pCreateString(pSize, pLength);
		
		for(from=pLength-1, to=0; to<pLength; from--, to++){
			string.pSetAt(to, pGetAt(from));
		}
	}
	
	return string;
}



decUnicodeStringList decUnicodeString::Split(int character) const{
	const int len = GetLength();
	decUnicodeStringList list;
	int i, start = -1;
	
	for(i=0; i<len; i++){
		if(start == -1){
			if(pGetAt(i) != character){
				start = i;
			}
			
		}else{
			if(pGetAt(i) == character){
				list.Add(GetMiddle(start, i));
				start = -1;
			}
		}
	}
	
	if(start != -1){
		list.Add(GetMiddle(start, len));
	}
	
	return list;
}

decUnicodeStringList decUnicodeString::Split(const decUnicodeString &characters) const{
	const int len = GetLength();
	decUnicodeStringList list;
	int i, j, start = -1;
	
	for(i=0; i<len; i++){
		if(start == -1){
			for(j=0; j<characters.pLength; j++){
				if(pGetAt(i) == characters.pGetAt(j)){
					break;
				}
			}
			if(j == characters.pLength){
				start = i;
			}
			
		}else{
			for(j=0; j<characters.pLength; j++){
				if(pGetAt(i) == characters.pGetAt(j)){
					list.Add(GetMiddle(start, i));
					start = -1;
					break;
				}
			}
		}
	}
	
	if(start != -1){
		list.Add(GetMiddle(start, len));
	}
	
	return list;
}



void decUnicodeString::Replace(int replaceCharacter, int withCharacter){
	const int len = GetLength();
	int i;
	
	pGrowString(pCharSize(withCharacter), pLength);
	
	for(i=0; i<len; i++){
		if(pGetAt(i) == replaceCharacter){
			pSetAt(i, withCharacter);
		}
	}
}

void decUnicodeString::Replace(const decUnicodeString &replaceCharacters, int withCharacter){
	int i;
	
	for(i=0; i<replaceCharacters.pLength; i++){
		Replace(replaceCharacters.pGetAt(i), withCharacter);
	}
}

void decUnicodeString::ReplaceString(const decUnicodeString &replaceString, const decUnicodeString &withString){
	const int len = pLength - replaceString.pLength + 1;
	
	if(replaceString.pLength > 0 && len > 0){
		const int difflen = withString.pLength - replaceString.pLength;
		const decUnicodeString string = *this;
		int newlen = pLength;
		int i, j, npos;
		
		for(i=0; i<len; i++){
			for(j=0; j<replaceString.pLength; j++){
				if(pGetAt(i + j) != replaceString.pGetAt(j)){
					break;
				}
			}
			if(j == replaceString.pLength){
				i += replaceString.pLength - 1;
				newlen += difflen;
			}
		}
		
		pCreateString(withString.pSize > string.pSize ? withString.pSize : string.pSize, newlen);
		
		for(npos=0, i=0; i<len; i++){
			for(j=0; j<replaceString.pLength; j++){
				if(string.pGetAt(i + j) != replaceString.pGetAt(j)){
					break;
				}
			}
			if(j == replaceString.pLength){
				for(j=0; j<withString.pLength; j++){
					pSetAt(npos++, withString.pGetAt(j));
				}
				i += replaceString.pLength - 1;
				
			}else{
				pSetAt(npos++, string.pGetAt(i));
			}
		}
		for(i=0; i<replaceString.pLength-1; i++){
			pSetAt(npos + i, string.pGetAt(len + i));
		}
	}
}

decUnicodeString decUnicodeString::GetReplaced(int replaceCharacter, int withCharacter) const{
	decUnicodeString string;
	
	if(pLength > 0){
		int i;
		
		const int withSize = pCharSize(withCharacter);
		string.pCreateString(pSize > withSize ? pSize : withSize, pLength);
		
		for(i=0; i<pLength; i++){
			if(pGetAt(i) == replaceCharacter){
				string.pSetAt(i, withCharacter);
				
			}else{
				string.pSetAt(i, pGetAt(i));
			}
		}
	}
	
	return string;
}

decUnicodeString decUnicodeString::GetReplaced(const decUnicodeString &replaceCharacters, int withCharacter) const{
	decUnicodeString string;
	
	if(replaceCharacters.pLength > 0){
		int i;
		
		string = GetReplaced(replaceCharacters.GetAt(0), withCharacter);
		
		for(i=1; i<replaceCharacters.pLength; i++){
			string.Replace(replaceCharacters.GetAt(i), withCharacter);
		}
	}
	
	return string;
}

decUnicodeString decUnicodeString::GetReplacedString(const decUnicodeString &replaceString, const decUnicodeString &withString) const{
	const int len = pLength - replaceString.pLength + 1;
	decUnicodeString string;
	
	if(replaceString.pLength > 0 && len > 0){
		const int difflen = withString.pLength - replaceString.pLength;
		int newlen = pLength;
		int i, j, npos;
		
		for(i=0; i<len; i++){
			for(j=0; j<replaceString.pLength; j++){
				if(pGetAt(i + j) != replaceString.pGetAt(j)){
					break;
				}
			}
			if(j == replaceString.pLength){
				i += replaceString.pLength - 1;
				newlen += difflen;
			}
		}
		
		string.pCreateString(pSize > withString.pSize ? pSize : withString.pSize, newlen);
		
		for(npos=0, i=0; i<len; i++){
			for(j=0; j<replaceString.pLength; j++){
				if(pGetAt(i + j) != replaceString.pGetAt(j)){
					break;
				}
			}
			if(j == replaceString.pLength){
				for(j=0; j<withString.pLength; j++){
					string.pSetAt(npos++, withString.pGetAt(j));
				}
				i += replaceString.pLength - 1;
				
			}else{
				string.pSetAt(npos++, pGetAt(i));
			}
		}
		for(i=0; i<replaceString.pLength-1; i++){
			string.pSetAt(npos + i, pGetAt(len + i));
		}
		
	}else{
		string = *this;
	}
	
	return string;
}



void decUnicodeString::TrimLeft(){
	int i, j;
	
	for(i=0; i<pLength; i++){
		if(!pIsSpace(pGetAt(i))){
			break;
		}
	}
	
	for(j=i; j<pLength; j++){
		pSetAt(j - i, pGetAt(j));
	}
	pLength -= i;
}

decUnicodeString decUnicodeString::GetTrimmedLeft() const{
	int i;
	
	for(i=0; i<pLength; i++){
		if(!pIsSpace(pGetAt(i))){
			break;
		}
	}
	
	return GetMiddle(i, pLength);
}

void decUnicodeString::TrimRight(){
	int i;
	
	for(i=pLength-1; i>=0; i--){
		if(!pIsSpace(pGetAt(i))){
			pLength = i + 1;
			break;
		}
	}
}

decUnicodeString decUnicodeString::GetTrimmedRight() const{
	int i;
	
	for(i=pLength-1; i>=0; i--){
		if(!pIsSpace(pGetAt(i))){
			break;
		}
	}
	
	return GetLeft(i + 1);
}

void decUnicodeString::Trim(){
	int start, end, i;
	
	for(start=0; start<pLength; start++){
		if(!pIsSpace(pGetAt(start))){
			break;
		}
	}
	
	for(end=pLength-1; end>=start; end--){
		if(!pIsSpace(pGetAt(end))){
			break;
		}
	}
	
	for(i=start; i<=end; i++){
		pSetAt(i - start, pGetAt(i));
	}
	pLength = end - start + 1;
}

decUnicodeString decUnicodeString::GetTrimmed() const{
	int start, end;
	
	for(start=0; start<pLength; start++){
		if(!pIsSpace(pGetAt(start))){
			break;
		}
	}
	
	for(end=pLength-1; end>=start; end--){
		if(!pIsSpace(pGetAt(end))){
			break;
		}
	}
	
	return GetMiddle(start, end + 1);
}



void decUnicodeString::ToLower(){
	int i;
	
	for(i=0; i<pLength; i++){
		pSetAt(i, pToLower(pGetAt(i)));
	}
}

decUnicodeString decUnicodeString::GetLower() const{
	decUnicodeString string;
	
	if(pLength > 0){
		int i;
		
		string.pCreateString(pSize, pLength);
		
		for(i=0; i<pLength; i++){
			string.pSetAt(i, pToLower(pGetAt(i)));
		}
	}
	
	return string;
}

void decUnicodeString::ToUpper(){
	int i;
	
	for(i=0; i<pLength; i++){
		pSetAt(i, pToUpper(pGetAt(i)));
	}
}

decUnicodeString decUnicodeString::GetUpper() const{
	decUnicodeString string;
	
	if(pLength > 0){
		int i;
		
		string.pCreateString(pSize, pLength);
		
		for(i=0; i<pLength; i++){
			string.pSetAt(i, pToUpper(pGetAt(i)));
		}
	}
	
	return string;
}



int decUnicodeString::ToInt() const{
	return ToUTF8().ToInt();
}

long long decUnicodeString::ToLong() const{
	return ToUTF8().ToLong();
}

float decUnicodeString::ToFloat() const{
	return ToUTF8().ToFloat();
}

double decUnicodeString::ToDouble() const{
	return ToUTF8().ToDouble();
}

decString decUnicodeString::ToUTF8() const{
	// determine required buffer length
	int utflen = 0;
	int i;
	
	for(i=0; i<pLength; i++){
		const int character = pGetAt(i);
		
		if(character < 0x80){
			utflen++;
			
		}else if(character < 0x800){
			utflen += 2;
			
		}else if(character < 0x10000){
			utflen += 3;
			
		}else{
			utflen += 4;
		}
	}
	
	// create string with the required size
	decString string;
	string.Set(' ', utflen);
	char *buffer = string.GetMutableString();
	
	for(i=0; i<pLength; i++){
		const int character = pGetAt(i);
		
		if(character < 0x80){
			*buffer++ = ( char )( unsigned char )character;
			
		}else if(character < 0x800){
			*buffer++ = ( char )( 0xc0 | ( unsigned char )( ( character >> 6 ) & 0x1f ) );
			*buffer++ = ( char )( 0x80 | ( unsigned char )( character & 0x3f ) );
			
		}else if(character < 0x10000){
			*buffer++ = ( char )( 0xe0 | ( unsigned char )( ( character >> 12 ) & 0xf ) );
			*buffer++ = ( char )( 0x80 | ( unsigned char )( ( character >> 6 ) & 0x3f ) );
			*buffer++ = ( char )( 0x80 | ( unsigned char )( character & 0x3f ) );
			
		}else{
			*buffer++ = ( char )( 0xf0 | ( unsigned char )( ( character >> 18 ) & 0x7 ) );
			*buffer++ = ( char )( 0x80 | ( unsigned char )( ( character >> 12 ) & 0x3f ) );
			*buffer++ = ( char )( 0x80 | ( unsigned char )( ( character >> 6 ) & 0x3f ) );
			*buffer++ = ( char )( 0x80 | ( unsigned char )( character & 0x3f ) );
		}
	}
	
	return string;
}



bool decUnicodeString::Equals(const decUnicodeString &string) const{
	return Compare(string) == 0;
}

bool decUnicodeString::EqualsInsensitive(const decUnicodeString &string) const{
	return CompareInsensitive(string) == 0;
}

int decUnicodeString::Compare(const decUnicodeString &string) const{
	int i;

	for(i=0; i<pLength; i++){
		if(i == string.pLength){
			return pGetAt(i);
		}
		
		const int difference = pGetAt(i) - string.pGetAt(i);
		if(difference != 0){
			return difference;
		}
	}
	
	if(pLength < string.pLength){
		return -string.pGetAt(pLength);
		
	}else{
		return 0;
	}
}

int decUnicodeString::CompareInsensitive(const decUnicodeString &string) const{
	int i;
	
	for(i=0; i<pLength; i++){
		if(i == string.pLength){
			return pGetAt(i);
		}
		
		const int character1 = pToLower(pGetAt(i));
		const int character2 = pToLower(string.pGetAt(i));
		const int difference = character1 - character2;
		if(difference != 0){
			return difference;
		}
	}
	
	if(pLength < string.pLength){
		return -string.pGetAt(pLength);
		
	}else{
		return 0;
	}
}

unsigned int decUnicodeString::Hash() const{
	// SDBM hash algorithm applied on the UTF8 version of the string
	unsigned int hash = 0;
	int i;
	
	for(i=0; i<pLength; i++){
		const int character = pGetAt(i);
		
		if(character < 0x80){
			hash = character + (hash << 6) + (hash << 16) - hash;
			
		}else if(character < 0x800){
			hash = (0xc0 | (unsigned char)((character >> 6) & 0x1f)) + (hash << 6) + (hash << 16) - hash;
			hash = (0x80 | (unsigned char)(character & 0x3f)) + (hash << 6) + (hash << 16) - hash;
			
		}else if(character < 0x10000){
			hash = (0xe0 | (unsigned char)((character >> 12) & 0xf)) + (hash << 6) + (hash << 16) - hash;
			hash = (0x80 | (unsigned char)((character >> 6) & 0x3f)) + (hash << 6) + (hash << 16) - hash;
			hash = (0x80 | (unsigned char)(character & 0x3f)) + (hash << 6) + (hash << 16) - hash;
			
		}else{
			hash = (0xf0 | (unsigned char)((character >> 18) & 0x7)) + (hash << 6) + (hash << 16) - hash;
			hash = (0x80 | (unsigned char)((character >> 12) & 0x3f)) + (hash << 6) + (hash << 16) - hash;
			hash = (0x80 | (unsigned char)((character >> 6) & 0x3f)) + (hash << 6) + (hash << 16) - hash;
			hash = (0x80 | (unsigned char)(character & 0x3f)) + (hash << 6) + (hash << 16) - hash;
		}
	}
	
	return hash;
}



// Operators
//////////////

int decUnicodeString::operator[](int position) const{
	if(position < 0){
		position += pLength;
	}
	if(position < 0 || position >= pLength){
		DETHROW(deeInvalidParam);
	}
	
	return pGetAt(position);
}

bool decUnicodeString::operator!() const{
	return pLength == 0;
}

bool decUnicodeString::operator==(const decUnicodeString &string) const{
	return Compare(string) == 0;
}

bool decUnicodeString::operator!=(const decUnicodeString &string) const{
	return Compare(string) != 0;
}

bool decUnicodeString::operator<(const decUnicodeString &string) const{
	return Compare(string) < 0;
}

bool decUnicodeString::operator<=(const decUnicodeString &string) const{
	return Compare(string) <= 0;
}

bool decUnicodeString::operator>(const decUnicodeString &string) const{
	return Compare(string) > 0;
}

bool decUnicodeString::operator>=(const decUnicodeString &string) const{
	return Compare(string) >= 0;
}



decUnicodeString decUnicodeString::operator+(const decUnicodeString &string) const{
	return decUnicodeString(*this, string);
}



decUnicodeString &decUnicodeString::operator=(const decUnicodeString &string){
	Set(string);
	return *this;
}

decUnicodeString &decUnicodeString::operator+=(const decUnicodeString &string){
	Append(string);
	return *this;
}



// Private functions
//////////////////////

int decUnicodeString::pGetAt(int position) const{
	if(pSize == 1){
		return *(((unsigned char*)pString) + position);
		
	}else if(pSize == 2){
		return *(((unsigned short*)pString) + position);
		
	}else{
		return *(((unsigned int*)pString) + position);
	}
}

void decUnicodeString::pSetAt(int position, int character){
	if(pSize == 1){
		*( ( ( unsigned char* )pString ) + position ) = ( unsigned char )character;
		
	}else if(pSize == 2){
		*( ( ( unsigned short* )pString ) + position ) = ( unsigned short )character;
		
	}else{
		*( ( ( unsigned int* )pString ) + position ) = character;
	}
}

void decUnicodeString::pFreeString(){
	if(pString){
		if(pSize == 1){
			delete [] ((unsigned char *)pString);
			
		}else if(pSize == 2){
			delete [] ((unsigned short *)pString);
			
		}else{
			delete [] ((unsigned int *)pString);
		}
		pString = NULL;
	}
	pLength = 0;
}

void decUnicodeString::pCreateString(int size, int length){
	pFreeString();
	
	if(length > 0){
		pString = new char[length * size];
		if(!pString){
			DETHROW(deeOutOfMemory);
		}
	}
	pSize = size;
	pLength = length;
}

void decUnicodeString::pGrowString(int size, int length){
	if(size > pSize || length > pLength){
		decUnicodeString old;
		int i;
		
		// store pointers away ( will be deleted on cleanup )
		old.pString = pString;
		old.pSize = pSize;
		old.pLength = pLength; 
		
		// create string
		pString = NULL;
		pCreateString(size > pSize ? size : pSize, length > pLength ? length : pLength);
		for(i=0; i<old.pLength; i++){
			pSetAt(i, old.pGetAt(i));
		}
	}
}

bool decUnicodeString::pIsSpace(int character) const{
	if(character >= 0x9 && character <= 0xd){
		return true;
	}
	if(character == 0x20){
		return true;
	}
	return false;
}

int decUnicodeString::pCharSize(int character) const{
	if(character > 0xffff){
		return 4;
		
	}else if(character > 0xff){
		return 2;
		
	}else{
		return 1;
	}
}

/* 
 * Drag[en]gine Game Engine Distribution Launcher
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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

#include <string.h>
#include <stdio.h>
#include "String.h"
#include "Exception.h"

String::String(){
	pString = new char[1];
	pString[0] = 0;
}

String::String(const String &string){
	pString = new char[string.Length() + 1];
	strcpy(pString, string.pString);
}

String::String(const char *string){
	pString = new char[strlen(string) + 1];
	strcpy(pString, string);
}

String::~String(){
	if(pString){
		delete [] pString;
	}
}

const char *String::Pointer() const{
	return pString;
}

int String::Length() const{
	return strlen(pString);
}

int String::Find(char delimiter) const{
	const char *s = strchr(pString, delimiter);
	if(s){
		return (int)(s - pString);
	}
	return -1;
}

String String::SubString(int from) const{
	return SubString(from, Length());
}

String String::SubString(int from, int to) const{
	const int len = Length();
	if(from < 0){
		from += len;
	}
	if(to < 0){
		to += len;
	}
	
	if(from < 0 || from >= len || to < 0 || to > len){
		throw Exception("Index out of bounds");
	}
	
	const int nlen = to - from;
	String s;
	delete [] s.pString;
	s.pString = new char[nlen + 1];
	strncpy(s.pString, pString + from, nlen);
	s.pString[nlen] = 0;
	return s;
}

String::operator const char*() const{
	return pString;
}

char String::operator[](int index) const{
	if(index < 0){
		index += Length();
	}
	if(index < 0 || index >= Length()){
		throw Exception("Index out of range");
	}
	return pString[index];
}

String String::operator+(const String &string) const{
	const int len = Length();
	String nstr;
	delete [] nstr.pString;
	nstr.pString = new char[len + string.Length() + 1];
	strcpy(nstr.pString, pString);
	strcpy(nstr.pString + len, string.pString);
	return nstr;
}

String String::operator+(char character) const{
	const int len = Length();
	String nstr;
	delete [] nstr.pString;
	nstr.pString = new char[len + 2];
	strcpy(nstr.pString, pString);
	nstr.pString[len] = character;
	nstr.pString[len + 1] = 0;
	return nstr;
}

String String::operator=(const String &string){
	if(pString){
		delete [] pString;
	}
	pString = new char[string.Length() + 1];
	strcpy(pString, string.pString);
	return *this;
}

String String::operator+=(const String &string){
	return *this = *this + string;
}

String String::operator+=(char character){
	return *this = *this + character;
}

bool String::operator==(const String &string) const{
	return strcmp(pString, string.pString) == 0;
}

bool String::operator==(const char *string) const{
	return strcmp(pString, string) == 0;
}

bool String::operator!=(const String &string) const{
	return strcmp(pString, string.pString) != 0;
}

bool String::operator!=(const char *string) const{
	return strcmp(pString, string) != 0;
}

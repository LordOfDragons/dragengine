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

#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "igdeLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



// Class igdeLoggerHistoryEntry
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeLoggerHistoryEntry::igdeLoggerHistoryEntry() :
pType(emtInfo){
}

igdeLoggerHistoryEntry::igdeLoggerHistoryEntry(const igdeLoggerHistoryEntry &entry) :
pType(entry.pType),
pSource(entry.pSource),
pMessage(entry.pMessage){
}

igdeLoggerHistoryEntry::igdeLoggerHistoryEntry(igdeLoggerHistoryEntry &&entry) :
pType(entry.pType),
pSource(std::move(entry.pSource)),
pMessage(std::move(entry.pMessage)){
}

igdeLoggerHistoryEntry::~igdeLoggerHistoryEntry(){
}


// Management
///////////////

void igdeLoggerHistoryEntry::SetType(eMessageTypes type){
	pType = type;
}

void igdeLoggerHistoryEntry::SetSource(const char *source){
	pSource = source;
}

void igdeLoggerHistoryEntry::SetMessage(const char *message){
	pMessage = message;
}

void igdeLoggerHistoryEntry::Clear(){
	pType = emtInfo;
	pSource.Empty();
	pMessage.Empty();
}

void igdeLoggerHistoryEntry::CleanUpMessage(){
	const int len = pMessage.GetLength();
	int found;
	
	for(found=len; found>0; found--){
		const int character = pMessage.GetAt(found - 1);
		
		if(!isspace(character) && character != '\n' && character != '\r'){
			break;
		}
	}
	
	if(found < len){
		pMessage.SetAt(found, '\0');
	}
}

igdeLoggerHistoryEntry &igdeLoggerHistoryEntry::operator=(const igdeLoggerHistoryEntry &entry){
	if(this != &entry){
		pType = entry.pType;
		pSource = entry.pSource;
		pMessage = entry.pMessage;
	}
	return *this;
}

igdeLoggerHistoryEntry &igdeLoggerHistoryEntry::operator=(igdeLoggerHistoryEntry &&entry) noexcept{
	if(this != &entry){
		pType = entry.pType;
		pSource = std::move(entry.pSource);
		pMessage = std::move(entry.pMessage);
	}
	return *this;
}

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

#include "delLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



// Class delLoggerHistoryEntry
/////////////////////////////////

// Constructor, destructor
////////////////////////////

delLoggerHistoryEntry::delLoggerHistoryEntry() :
pType(emtInfo){
}

delLoggerHistoryEntry::delLoggerHistoryEntry(const delLoggerHistoryEntry &entry) :
pTimeStamp(entry.pTimeStamp),
pType(entry.pType),
pSource(entry.pSource),
pMessage(entry.pMessage){
}

delLoggerHistoryEntry::~delLoggerHistoryEntry(){
}



// Management
///////////////

void delLoggerHistoryEntry::SetTimeStamp(const decDateTime &timestamp){
	pTimeStamp = timestamp;
}

void delLoggerHistoryEntry::SetType(eMessageTypes type){
	pType = type;
}

void delLoggerHistoryEntry::SetSource(const decString &source){
	pSource = source;
}

void delLoggerHistoryEntry::SetMessage(const decString &message){
	pMessage = message;
}

void delLoggerHistoryEntry::Clear(){
	pTimeStamp = decDateTime();
	pType = emtInfo;
	pSource.Empty();
	pMessage.Empty();
}

void delLoggerHistoryEntry::CleanUpMessage(){
	int len = pMessage.GetLength();
	int found;
	
	for(found=len; found>0; found--){
		const int character = pMessage.GetAt(found - 1);
		if(! isspace(character) && character != '\n' && character != '\r'){
			break;
		}
	}
	
	if(found < len){
		pMessage.SetAt(found, '\0');
	}
}



// Operators
//////////////

delLoggerHistoryEntry &delLoggerHistoryEntry::operator=(const delLoggerHistoryEntry &entry){
	pTimeStamp = entry.pTimeStamp;
	pType = entry.pType;
	pSource = entry.pSource;
	pMessage = entry.pMessage;
	return *this;
}

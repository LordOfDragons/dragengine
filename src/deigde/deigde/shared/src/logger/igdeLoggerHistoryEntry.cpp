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

igdeLoggerHistoryEntry::igdeLoggerHistoryEntry(){
	pType = emtInfo;
}

igdeLoggerHistoryEntry::~igdeLoggerHistoryEntry(){
}



// Management
///////////////

void igdeLoggerHistoryEntry::SetType( int type ){
	if( type < emtInfo || type > emtError ) DETHROW( deeInvalidParam );
	
	pType = type;
}

void igdeLoggerHistoryEntry::Clear(){
	pType = emtInfo;
	pSource.Empty();
	pMessage.Empty();
}

void igdeLoggerHistoryEntry::CleanUpMessage(){
	int len = pMessage.GetLength();
	int found, character;
	
	for( found=len; found>0; found-- ){
		character = pMessage.GetAt( found - 1 );
		
		if( ! isspace( character ) && character != '\n' && character != '\r' ){
			break;
		}
	}
	
	if( found < len ){
		pMessage.SetAt( found, '\0' );
	}
}

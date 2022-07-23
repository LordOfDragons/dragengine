/* 
 * Drag[en]gine Launcher Shared
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
pType( emtInfo ){
}

delLoggerHistoryEntry::delLoggerHistoryEntry( const delLoggerHistoryEntry &entry ) :
pTimeStamp( entry.pTimeStamp ),
pType( entry.pType ),
pSource( entry.pSource ),
pMessage( entry.pMessage ){
}

delLoggerHistoryEntry::~delLoggerHistoryEntry(){
}



// Management
///////////////

void delLoggerHistoryEntry::SetTimeStamp( const decDateTime &timestamp ){
	pTimeStamp = timestamp;
}

void delLoggerHistoryEntry::SetType( eMessageTypes type ){
	pType = type;
}

void delLoggerHistoryEntry::SetSource( const decString &source ){
	pSource = source;
}

void delLoggerHistoryEntry::SetMessage( const decString &message ){
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
	
	for( found=len; found>0; found-- ){
		const int character = pMessage.GetAt( found - 1 );
		if( ! isspace( character ) && character != '\n' && character != '\r' ){
			break;
		}
	}
	
	if( found < len ){
		pMessage.SetAt( found, '\0' );
	}
}



// Operators
//////////////

delLoggerHistoryEntry &delLoggerHistoryEntry::operator=( const delLoggerHistoryEntry &entry ){
	pTimeStamp = entry.pTimeStamp;
	pType = entry.pType;
	pSource = entry.pSource;
	pMessage = entry.pMessage;
	return *this;
}

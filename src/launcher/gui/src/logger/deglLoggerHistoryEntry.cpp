/* 
 * Drag[en]gine GUI Launcher
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#include "deglLoggerHistoryEntry.h"

#include <dragengine/common/exceptions.h>



// Class deglLoggerHistoryEntry
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deglLoggerHistoryEntry::deglLoggerHistoryEntry(){
	pType = emtInfo;
}

deglLoggerHistoryEntry::~deglLoggerHistoryEntry(){
}



// Management
///////////////

void deglLoggerHistoryEntry::SetType( int type ){
	if( type < emtInfo || type > emtError ) DETHROW( deeInvalidParam );
	
	pType = type;
}

void deglLoggerHistoryEntry::Clear(){
	pType = emtInfo;
	pSource.Empty();
	pMessage.Empty();
}

void deglLoggerHistoryEntry::CleanUpMessage(){
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

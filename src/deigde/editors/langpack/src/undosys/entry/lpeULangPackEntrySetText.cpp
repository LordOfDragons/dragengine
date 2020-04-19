/* 
 * Drag[en]gine IGDE Language Pack Editor
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "lpeULangPackEntrySetText.h"
#include "../../langpack/entry/lpeLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackEntrySetText
///////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackEntrySetText::lpeULangPackEntrySetText(
	lpeLangPackEntry *entry, const decUnicodeString &newName ) :
pEntry( NULL ),
pNewText( newName )
{
	if( ! entry ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Entry set text" );
	
	pOldText = entry->GetText();
	
	pEntry = entry;
	entry->AddReference();
}

lpeULangPackEntrySetText::~lpeULangPackEntrySetText(){
	if( pEntry ){
		pEntry->FreeReference();
	}
}



// Management
///////////////

void lpeULangPackEntrySetText::Undo(){
	pEntry->SetText( pOldText );
}

void lpeULangPackEntrySetText::Redo(){
	pEntry->SetText( pNewText );
}

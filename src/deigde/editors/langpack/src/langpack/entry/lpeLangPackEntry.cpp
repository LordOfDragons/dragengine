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
#include <stdlib.h>
#include <string.h>

#include "lpeLangPackEntry.h"
#include "../lpeLangPack.h"

#include <dragengine/common/exceptions.h>



// Class lpeLangPackEntry
///////////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPackEntry::lpeLangPackEntry(){
	pLangPack = NULL;
	
	pName = "Entry";
	
	pSelected = false;
	pActive = false;
}

lpeLangPackEntry::lpeLangPackEntry( const lpeLangPackEntry &entry ){
	pLangPack = NULL;
	
	pName = entry.pName;
	pText = entry.pText;
	
	pSelected = false;
	pActive = false;
}

lpeLangPackEntry::~lpeLangPackEntry(){
}



// Management
///////////////

void lpeLangPackEntry::SetLangPack( lpeLangPack *langpack ){
	pLangPack = langpack;
}



void lpeLangPackEntry::SetName( const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! pName.Equals( name ) ){
		pName = name;
		NotifyNameChanged();
	}
}

void lpeLangPackEntry::SetText( const decUnicodeString &text ){
	if( pText != text ){
		pText = text;
		NotifyChanged();
	}
}

void lpeLangPackEntry::SetSelected( bool selected ){
	pSelected = selected;
}

void lpeLangPackEntry::SetActive( bool active ){
	pActive = active;
}



void lpeLangPackEntry::NotifyChanged(){
	if( pLangPack ){
		pLangPack->NotifyEntryChanged( this );
	}
}

void lpeLangPackEntry::NotifyNameChanged(){
	if( pLangPack ){
		pLangPack->NotifyEntryNameChanged( this );
	}
}

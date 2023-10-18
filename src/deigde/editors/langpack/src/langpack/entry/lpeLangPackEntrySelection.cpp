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
#include "lpeLangPackEntryList.h"
#include "lpeLangPackEntrySelection.h"

#include <dragengine/common/exceptions.h>



// Class lpeLangPackEntrySelection
////////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPackEntrySelection::lpeLangPackEntrySelection(){
	pActive = NULL;
}

lpeLangPackEntrySelection::~lpeLangPackEntrySelection(){
	Reset();
}



// Management
///////////////

void lpeLangPackEntrySelection::Add( lpeLangPackEntry *entry ){
	DEASSERT_NOTNULL( entry )
	
	entry->SetSelected( true );
	
	if( ! pSelection.Has( entry ) ){
		pSelection.Add( entry );
	}
}

void lpeLangPackEntrySelection::Remove( lpeLangPackEntry *entry ){
	DEASSERT_NOTNULL( entry )
	
	entry->SetSelected( false );
	
	if( pSelection.Has( entry ) ){
		pSelection.Remove( entry );
	}
}

void lpeLangPackEntrySelection::RemoveAll(){
	const int count = pSelection.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pSelection.GetAt( i )->SetSelected( false );
	}
	
	pSelection.RemoveAll();
}



bool lpeLangPackEntrySelection::HasActive() const{
	return pActive != NULL;
}

void lpeLangPackEntrySelection::SetActive( lpeLangPackEntry *entry ){
	if( entry == pActive ){
		return;
	}
	
	if( entry ){
		DEASSERT_TRUE( pSelection.Has( entry ) )
		DEASSERT_TRUE( entry->GetSelected() )
	}
	
	if( pActive ){
		pActive->SetActive( false );
		pActive->FreeReference();
	}
	
	pActive = entry;
	
	if( entry ){
		entry->AddReference();
		entry->SetActive( true );
	}
}

void lpeLangPackEntrySelection::ActivateNext(){
	const int count = pSelection.GetCount();
	lpeLangPackEntry *next = NULL;
	int i;
	
	for( i=0; i<count; i++ ){
		if( pActive != pSelection.GetAt( i ) ){
			next = pSelection.GetAt( i );
			break;
		}
	}
	
	SetActive( next );
}

void lpeLangPackEntrySelection::Reset(){
	SetActive( nullptr );
	RemoveAll();
}

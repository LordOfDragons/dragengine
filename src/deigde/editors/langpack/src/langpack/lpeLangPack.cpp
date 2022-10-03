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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lpeLangPack.h"
#include "lpeLangPackBuilder.h"
#include "lpeLangPackListener.h"
#include "entry/lpeLangPackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/logger/deLogger.h>
#include <dragengine/common/exceptions.h>



// Definitions
////////////////

#define LOGSOURCE "Langiage Pack Editor"



// Class lpeLangPack
//////////////////////

// Constructor, destructor
////////////////////////////

lpeLangPack::lpeLangPack( igdeEnvironment *environment ) : igdeEditableEntity( environment ){
	pIdentifier = "id";
	pName.SetFromUTF8( "Language Pack" );
	pMissingText.SetFromUTF8( "??" );
}

lpeLangPack::~lpeLangPack(){
	RemoveAllEntries();
}



// Management
///////////////

void lpeLangPack::SetIdentifier( const char *identifier ){
	if( pIdentifier == identifier ){
		return;
	}
	
	pIdentifier = identifier;
	NotifyLangPackChanged();
}

void lpeLangPack::SetName( const decUnicodeString &name ){
	if( name != pName ){
		pName = name;
		NotifyLangPackChanged();
	}
}

void lpeLangPack::SetDescription( const decUnicodeString &description ){
	if( description != pDescription ){
		pDescription = description;
		NotifyLangPackChanged();
	}
}

void lpeLangPack::SetMissingText( const decUnicodeString &missingText ){
	if( missingText != pMissingText ){
		pMissingText = missingText;
		NotifyLangPackChanged();
	}
}



// Entries
/////////////

void lpeLangPack::AddEntry( lpeLangPackEntry *entry ){
	pEntryList.Add( entry );
	entry->SetLangPack( this );
	NotifyEntryStructureChanged();
}


void lpeLangPack::RemoveEntry( lpeLangPackEntry *entry ){
	if( ! entry || entry->GetLangPack() != this ){
		DETHROW( deeInvalidParam );
	}
	
	entry->SetLangPack( NULL );
	pEntryList.Remove( entry );
	NotifyEntryStructureChanged();
}

void lpeLangPack::RemoveAllEntries(){
	const int count = pEntryList.GetCount();
	int i;
	
	for( i =0; i <count; i++ ){
		pEntryList.GetAt( i )->SetLangPack( NULL );
	}
	pEntryList.RemoveAll();
	NotifyEntryStructureChanged();
}



// Listeners
//////////////

void lpeLangPack::AddListener( lpeLangPackListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	pListeners.Add( listener );
}

void lpeLangPack::RemoveListener( lpeLangPackListener *listener ){
	pListeners.RemoveIfPresent( listener );
}



void lpeLangPack::NotifyStateChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i =0; i <listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->StateChanged( this );
	}
}

void lpeLangPack::NotifyUndoChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i =0; i <listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->UndoChanged( this );
	}
}

void lpeLangPack::NotifyLangPackChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i =0; i <listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->LangPackChanged( this );
	}
	
	SetChanged( true );
}



void lpeLangPack::NotifyEntryStructureChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i =0; i <listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->EntryStructureChanged( this );
	}
	
	SetChanged( true );
}

void lpeLangPack::NotifyEntryChanged( lpeLangPackEntry *entry ){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i =0; i <listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->EntryChanged( this, entry );
	}
	
	SetChanged( true );
}

void lpeLangPack::NotifyEntryNameChanged( lpeLangPackEntry *entry ){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i=0; i<listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->EntryNameChanged( this, entry );
	}
	
	SetChanged( true );
}

void lpeLangPack::NotifyEntrySelectionChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i=0; i<listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->EntrySelectionChanged( this );
	}
}

void lpeLangPack::NotifyActiveEntryChanged(){
	const int listenerCount = pListeners.GetCount();
	int i;
	
	for( i =0; i <listenerCount; i++ ){
		( ( lpeLangPackListener* )pListeners.GetAt( i ) )->ActiveEntryChanged( this );
	}
}

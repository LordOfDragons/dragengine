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
	DEASSERT_NOTNULL( listener )
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

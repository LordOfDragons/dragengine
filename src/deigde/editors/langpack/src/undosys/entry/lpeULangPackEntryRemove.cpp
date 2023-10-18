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

#include "lpeULangPackEntryRemove.h"
#include "../../langpack/lpeLangPack.h"
#include "../../langpack/entry/lpeLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackEntryRemove
//////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackEntryRemove::lpeULangPackEntryRemove( lpeLangPack *langpack,
	const lpeLangPackEntryList &list, lpeLangPack *refLangpack ) :
pLangPack( langpack ),
pList( list )
{
	const int count = list.GetCount();
	DEASSERT_NOTNULL( langpack )
	DEASSERT_TRUE( count > 0 )
	
	if( refLangpack ){
		const lpeLangPackEntryList &refEntries = refLangpack->GetEntryList();
		int i;
		
		for( i=0; i<count; i++ ){
			lpeLangPackEntry * const refEntry = refEntries.GetNamed( list.GetAt( i )->GetName() );
			if( refEntry ){
				pListRef.Add( refEntry );
			}
		}
	}
	
	SetShortInfo( count == 1 ? "Remove entry" : "Remove entries" );
}

lpeULangPackEntryRemove::~lpeULangPackEntryRemove(){
}



// Management
///////////////

void lpeULangPackEntryRemove::Undo(){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	const int count = pList.GetCount();
	int i;
	
	lpes.Reset();
	
	for( i=0; i<count; i++ ){
		lpeLangPackEntry * const entry = pList.GetAt( i );
		pLangPack->AddEntry( entry );
		lpes.Add( entry );
	}
	
	lpes.ActivateNext();
	
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}

void lpeULangPackEntryRemove::Redo(){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		lpeLangPackEntry * const entry = pList.GetAt( i );
		
		lpes.Remove( entry );
		pLangPack->RemoveEntry( entry );
	}
	
	const int refCount = pListRef.GetCount();
	if( refCount > 0 ){
		for( i=0; i<refCount; i++ ){
			lpes.Add( pListRef.GetAt( i ) );
		}
		lpes.SetActive( pListRef.GetAt( 0 ) );
	}
	
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}

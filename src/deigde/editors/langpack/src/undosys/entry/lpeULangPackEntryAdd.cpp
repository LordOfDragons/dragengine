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

#include "lpeULangPackEntryAdd.h"
#include "../../langpack/lpeLangPack.h"
#include "../../langpack/entry/lpeLangPackEntry.h"

#include <dragengine/common/exceptions.h>



// Class lpeULangPackEntryAdd
///////////////////////////////

// Constructor, destructor
////////////////////////////

lpeULangPackEntryAdd::lpeULangPackEntryAdd( lpeLangPack *langpack,
	lpeLangPackEntry *entry, lpeLangPackEntry *refEntry ) :
pLangPack( langpack ),
pEntry( entry ),
pRefEntry( refEntry )
{
	DEASSERT_NOTNULL( langpack )
	DEASSERT_NOTNULL( entry )
	
	SetShortInfo( "Add entry" );
}

lpeULangPackEntryAdd::~lpeULangPackEntryAdd(){
}



// Management
///////////////

void lpeULangPackEntryAdd::Undo(){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	
	lpes.Remove( pEntry );
	
	if( pRefEntry ){
		lpes.Add( pRefEntry );
		lpes.SetActive( pRefEntry );
		
	}else{
		lpes.ActivateNext();
	}
	
	pLangPack->RemoveEntry( pEntry );
	
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}

void lpeULangPackEntryAdd::Redo(){
	lpeLangPackEntrySelection &lpes = pLangPack->GetEntrySelection();
	
	pLangPack->AddEntry( pEntry );
	
	lpes.Reset();
	lpes.Add( pEntry );
	lpes.SetActive( pEntry );
	
	pLangPack->NotifyEntrySelectionChanged();
	pLangPack->NotifyActiveEntryChanged();
}

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

#include "lpeViewLangPack.h"
#include "lpeViewLangPackListener.h"
#include "../langpack/entry/lpeLangPackEntry.h"

#include <deigde/environment/igdeEnvironment.h>

#include <dragengine/common/exceptions.h>



// Class lpeViewLangPackListener
//////////////////////////////////

// Constructor, destructor
////////////////////////////

lpeViewLangPackListener::lpeViewLangPackListener( lpeViewLangPack &panel ) :
pPanel( panel ){
}

lpeViewLangPackListener::~lpeViewLangPackListener(){
}



// Management
///////////////

void lpeViewLangPackListener::LangPackChanged( lpeLangPack *langpack ){
	if( pPanel.GetLangPack() != langpack ){
		return;
	}
	
	pPanel.UpdateEntries();
}



void lpeViewLangPackListener::EntryStructureChanged( lpeLangPack *langpack ){
	if( pPanel.GetLangPack() != langpack ){
		return;
	}
	
	pPanel.UpdateEntries();
}

void lpeViewLangPackListener::EntryChanged( lpeLangPack *langpack, lpeLangPackEntry *entry ){
	if( pPanel.GetLangPack() != langpack ){
		return;
	}
	
	if( entry->GetActive() ){
		pPanel.UpdateActiveEntry();
		
	}else{
		pPanel.UpdateEntry( entry );
	}
}

void lpeViewLangPackListener::EntryNameChanged( lpeLangPack *langpack, lpeLangPackEntry *entry ){
	if( pPanel.GetLangPack() != langpack ){
		return;
	}
	
	if( entry->GetActive() ){
		pPanel.UpdateActiveEntry();
	}
	pPanel.SortEntries();
}

void lpeViewLangPackListener::EntrySelectionChanged( lpeLangPack *langpack ){
	if( pPanel.GetLangPack() != langpack ){
		return;
	}
	
	pPanel.UpdateEntrySelection();
}

void lpeViewLangPackListener::ActiveEntryChanged( lpeLangPack *langpack ){
	if( pPanel.GetLangPack() != langpack ){
		return;
	}
	
	pPanel.SelectActiveEntry();
}

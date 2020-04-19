/* 
 * Drag[en]gine IGDE World Editor
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

#include "meUDecalPropertyRemoveFromSelected.h"
#include "meUndoDataDecalProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalPropertyRemoveFromSelected
/////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalPropertyRemoveFromSelected::meUDecalPropertyRemoveFromSelected( const meDecalList &list, const char *key ){
	meUndoDataDecalProperty *undoData = NULL;
	const int count = list.GetCount();
	meDecal *decal;
	int i;
	
	if( ! key || count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		SetShortInfo( "Remove decal property from selected" );
		pKey = key;
		
		for( i=0; i<count; i++ ){
			decal = list.GetAt( i );
			if( ! decal->GetWorld() ){
				DETHROW( deeInvalidParam );
			}
			
			const decStringDictionary &properties = decal->GetProperties();
			
			undoData = new meUndoDataDecalProperty( decal );
			undoData->SetPropertyExists( properties.Has( key ) );
			if( undoData->GetPropertyExists() ){
				undoData->SetOldValue( properties.GetAt( key ) );
			}
			
			pList.Add( undoData );
			undoData->FreeReference();
			undoData = NULL;
		}
		
	}catch( const deException & ){
		if( undoData ){
			undoData->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

meUDecalPropertyRemoveFromSelected::~meUDecalPropertyRemoveFromSelected(){
	pCleanUp();
}



// Management
///////////////

void meUDecalPropertyRemoveFromSelected::Undo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataDecalProperty &undoData = *( ( meUndoDataDecalProperty* )pList.GetAt( i ) );
		
		if( undoData.GetPropertyExists() ){
			undoData.GetDecal()->SetProperty( pKey, undoData.GetOldValue() );
		}
	}
}

void meUDecalPropertyRemoveFromSelected::Redo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataDecalProperty &undoData = *( ( meUndoDataDecalProperty* )pList.GetAt( i ) );
		
		if( undoData.GetPropertyExists() ){
			undoData.GetDecal()->RemoveProperty( pKey );
		}
	}
}



// Private Functions
//////////////////////

void meUDecalPropertyRemoveFromSelected::pCleanUp(){
	pList.RemoveAll();
}

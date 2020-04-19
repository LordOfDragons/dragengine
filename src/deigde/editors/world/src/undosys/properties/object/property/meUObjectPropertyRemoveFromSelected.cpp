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

#include "meUObjectPropertyRemoveFromSelected.h"
#include "meUndoDataObjectProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectPropertyRemoveFromSelected
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectPropertyRemoveFromSelected::meUObjectPropertyRemoveFromSelected( const meObjectList &list, const char *key ){
	meUndoDataObjectProperty *undoData = NULL;
	const int count = list.GetCount();
	meObject *object;
	int i;
	
	if( ! key || count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		SetShortInfo( "Remove object property from selected" );
		pKey = key;
		
		for( i=0; i<count; i++ ){
			object = list.GetAt( i );
			if( ! object->GetWorld() ){
				DETHROW( deeInvalidParam );
			}
			
			const decStringDictionary &properties = object->GetProperties();
			
			undoData = new meUndoDataObjectProperty( object );
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

meUObjectPropertyRemoveFromSelected::~meUObjectPropertyRemoveFromSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectPropertyRemoveFromSelected::Undo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObjectProperty &undoData = *( ( meUndoDataObjectProperty* )pList.GetAt( i ) );
		
		if( undoData.GetPropertyExists() ){
			undoData.GetObject()->SetProperty( pKey, undoData.GetOldValue() );
		}
	}
}

void meUObjectPropertyRemoveFromSelected::Redo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObjectProperty &undoData = *( ( meUndoDataObjectProperty* )pList.GetAt( i ) );
		
		if( undoData.GetPropertyExists() ){
			undoData.GetObject()->RemoveProperty( pKey );
		}
	}
}



// Private Functions
//////////////////////

void meUObjectPropertyRemoveFromSelected::pCleanUp(){
	pList.RemoveAll();
}

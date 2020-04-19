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

#include "meUObjectPropertyCopyToSelected.h"
#include "meUndoDataObjectProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectPropertyCopyToSelected
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectPropertyCopyToSelected::meUObjectPropertyCopyToSelected( const meObjectList &list, const char *key, const char *value ){
	meUndoDataObjectProperty *undoData = NULL;
	const int count = list.GetCount();
	meObject *object;
	int i;
	
	if( ! key || ! value || count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		SetShortInfo( "Copy object property to selected" );
		pKey = key;
		pValue = value;
		
		for( i=0; i<count; i++ ){
			object = list.GetAt( i );
			if( ! object->GetWorld() ){
				DETHROW( deeInvalidParam );
			}
			
			const decStringDictionary &properties= object->GetProperties();
			
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

meUObjectPropertyCopyToSelected::~meUObjectPropertyCopyToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectPropertyCopyToSelected::SetValue( const char *value ){
	pValue = value;
}



void meUObjectPropertyCopyToSelected::Undo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObjectProperty &undoData = *( ( meUndoDataObjectProperty* )pList.GetAt( i ) );
		
		if( undoData.GetPropertyExists() ){
			undoData.GetObject()->SetProperty( pKey.GetString(), undoData.GetOldValue().GetString() );
			
		}else{
			undoData.GetObject()->RemoveProperty( pKey.GetString() );
		}
	}
}

void meUObjectPropertyCopyToSelected::Redo(){
	const int count = pList.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObjectProperty &undoData = *( ( meUndoDataObjectProperty* )pList.GetAt( i ) );
		
		undoData.GetObject()->SetProperty( pKey.GetString(), pValue.GetString() );
	}
}



// Private Functions
//////////////////////

void meUObjectPropertyCopyToSelected::pCleanUp(){
	pList.RemoveAll();
}

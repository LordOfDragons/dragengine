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

#include "meUObjectTextureCopyToSelected.h"
#include "meUndoDataObjectTexture.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectTextureCopyToSelected
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectTextureCopyToSelected::meUObjectTextureCopyToSelected( const meObjectList &list, meObjectTexture *texture ){
	const char *textureName = texture->GetName().GetString();
	meUndoDataObjectTexture *undoData = NULL;
	const int count = list.GetCount();
	meObject *object;
	int i;
	
	if( ! texture || count < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	
	try{
		SetShortInfo( "Copy object texture to selected" );
		
		for( i=0; i<count; i++ ){
			object = list.GetAt( i );
			if( ! object->GetWorld() ){
				DETHROW( deeInvalidParam );
			}
			
			undoData = new meUndoDataObjectTexture( object );
			undoData->SetOldTexture( object->GetTextureNamed( textureName ) );
			
			if( undoData->GetOldTexture() != texture ){
				pList.Add( undoData );
			}
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
	
	pTexture = texture;
	texture->AddReference();
}

meUObjectTextureCopyToSelected::~meUObjectTextureCopyToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectTextureCopyToSelected::Undo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		undoData = ( meUndoDataObjectTexture* )pList.GetAt( i );
		object = undoData->GetObject();
		
		if( undoData->GetNewTexture() ){
			object->RemoveTexture( undoData->GetNewTexture() );
		}
		if( undoData->GetOldTexture() ){
			object->AddTexture( undoData->GetOldTexture() );
		}
	}
}

void meUObjectTextureCopyToSelected::Redo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	meObjectTexture *texture = NULL;
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		undoData = ( meUndoDataObjectTexture* )pList.GetAt( i );
		object = undoData->GetObject();
		
		if( ! undoData->GetNewTexture() ){
			try{
				texture = new meObjectTexture( *pTexture );
				undoData->SetNewTexture( texture );
				texture->FreeReference();
				texture = NULL;
				
			}catch( const deException & ){
				if( texture ){
					texture->FreeReference();
				}
				throw;
			}
		}
		
		if( undoData->GetOldTexture() ){
			object->RemoveTexture( undoData->GetOldTexture() );
		}
		object->AddTexture( undoData->GetNewTexture() );
	}
}



// Private Functions
//////////////////////

void meUObjectTextureCopyToSelected::pCleanUp(){
	pList.RemoveAll();
}

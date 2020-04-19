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

#include "meUObjectCloneTexturesToSelected.h"
#include "meUndoDataObjectTexture.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCloneTexturesToSelected
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCloneTexturesToSelected::meUObjectCloneTexturesToSelected(
const meObjectList &list, const meObjectTextureList &textureList ){
	int textureCount = textureList.GetTextureCount();
	meUndoDataObjectTexture *undoData = NULL;
	const int objectCount = list.GetCount();
	meObject *object;
	int i, j;
	
	if( objectCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		SetShortInfo( "Clone object textures to selected" );
		
		for( i=0; i<textureCount; i++ ){
			pTextureList.AddTexture( textureList.GetTextureAt( i ) );
		}
		
		for( i=0; i<objectCount; i++ ){
			object = list.GetAt( i );
			
			if( ! object->GetWorld() ){
				DETHROW( deeInvalidParam );
			}
			
			undoData = new meUndoDataObjectTexture( object );
			textureCount = object->GetTextureCount();
			for( j=0; j<textureCount; j++ ){
				undoData->GetOldTextureList().AddTexture( object->GetTextureAt( j ) );
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

meUObjectCloneTexturesToSelected::~meUObjectCloneTexturesToSelected(){
	pCleanUp();
}



// Management
///////////////

void meUObjectCloneTexturesToSelected::Undo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	int i, j, textureCount;
	meObject *object;
	
	for( i=0; i<count; i++ ){
		undoData = ( meUndoDataObjectTexture* )pList.GetAt( i );
		meObjectTextureList &oldTextureList = undoData->GetOldTextureList();
		object = undoData->GetObject();
		
		object->RemoveAllTextures();
		textureCount = oldTextureList.GetTextureCount();
		for( j=0; j<textureCount; j++ ){
			object->AddTexture( oldTextureList.GetTextureAt( j ) );
		}
	}
}

void meUObjectCloneTexturesToSelected::Redo(){
	const int count = pList.GetCount();
	meUndoDataObjectTexture *undoData;
	meObjectTexture *texture = NULL;
	int i, j, textureCount;
	meObject *object;
	
	for( i=0; i<count; i++ ){
		undoData = ( meUndoDataObjectTexture* )pList.GetAt( i );
		meObjectTextureList &newTextureList = undoData->GetNewTextureList();
		object = undoData->GetObject();
		
		if( newTextureList.GetTextureCount() == 0 ){
			textureCount = pTextureList.GetTextureCount();
			
			try{
				for( j=0; j<textureCount; j++ ){
					texture = new meObjectTexture( *pTextureList.GetTextureAt( j ) );
					newTextureList.AddTexture( texture );
					texture->FreeReference();
					texture = NULL;
				}
				
			}catch( const deException & ){
				if( texture ){
					texture->FreeReference();
				}
				throw;
			}
		}
		
		object->RemoveAllTextures();
		textureCount = newTextureList.GetTextureCount();
		for( j=0; j<textureCount; j++ ){
			object->AddTexture( newTextureList.GetTextureAt( j ) );
		}
	}
}



// Private Functions
//////////////////////

void meUObjectCloneTexturesToSelected::pCleanUp(){
	pList.RemoveAll();
}

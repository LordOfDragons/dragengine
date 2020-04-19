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
#include <string.h>

#include "meUObjectRemoveAllTextures.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"
#include "../../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectRemoveAllTextures
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectRemoveAllTextures::meUObjectRemoveAllTextures( meObject *object ){
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = object->GetTextureCount();
	int i;
	
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = object->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	pObject = NULL;
	
	SetShortInfo( "Remove All Object Textures" );
	
	for( i=0; i<count; i++ ){
		pTextureList.AddTexture( object->GetTextureAt( i ) );
	}
	
	pObject = object;
	object->AddReference();
}

meUObjectRemoveAllTextures::~meUObjectRemoveAllTextures(){
	pTextureList.RemoveAllTextures();
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUObjectRemoveAllTextures::Undo(){
	const int count = pTextureList.GetTextureCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pObject->AddTexture( pTextureList.GetTextureAt( i ) );
	}
}

void meUObjectRemoveAllTextures::Redo(){
	pObject->RemoveAllTextures();
}

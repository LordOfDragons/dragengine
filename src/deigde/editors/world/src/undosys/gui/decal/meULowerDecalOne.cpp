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

#include "meULowerDecalOne.h"
#include "../../../world/meWorld.h"
#include "../../../world/decal/meDecal.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"
#include "dragengine/common/exceptions.h"



// Class meULowerDecalOne
///////////////////////////

// Constructor, destructor
////////////////////////////

meULowerDecalOne::meULowerDecalOne( meWorld *world, meDecal *decal ){
	if( ! world || ! decal ) DETHROW( deeInvalidParam );
	
	if( ! decal->GetParentObject() ) DETHROW( deeInvalidParam );
	
	pWorld = world;
	
	pDecal = decal;
	decal->AddReference();
	
	SetShortInfo( "Lower decal one level." );
	SetLongInfo( "" );
}

meULowerDecalOne::~meULowerDecalOne(){
	if( pDecal ) pDecal->FreeReference();
}



// Undo and Redo operations
/////////////////////////////

void meULowerDecalOne::Undo(){
	meObject *object = pDecal->GetParentObject();
	int index;
	
	if( object ){
		index = object->IndexOfDecal( pDecal );
		if( index < object->GetDecalCount() - 1 ){
			object->MoveDecalTo( pDecal, index + 1 );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

void meULowerDecalOne::Redo(){
	meObject *object = pDecal->GetParentObject();
	int index;
	
	if( object ){
		index = object->IndexOfDecal( pDecal );
		if( index > 0 ){
			object->MoveDecalTo( pDecal, index - 1 );
		}
		
	}else{
		DETHROW( deeInvalidParam );
	}
}

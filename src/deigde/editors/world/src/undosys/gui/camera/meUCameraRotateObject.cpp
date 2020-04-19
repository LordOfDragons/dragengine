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

#include "meUCameraRotateObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../worldedit.h"

#include <dragengine/common/exceptions.h>



// Class meUCameraRotateObject
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUCameraRotateObject::meUCameraRotateObject( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pOldRotation = object->GetRotation();
	pNewRotation = pOldRotation;
	SetShortInfo( "Rotate Camera Object" );
	
	pObject = object;
	object->AddReference();
}

meUCameraRotateObject::~meUCameraRotateObject(){
	if( pObject ) pObject->FreeReference();
}



// Undo and Redo operations
/////////////////////////////

void meUCameraRotateObject::SetNewRotation( const decVector &rotation ){
	pNewRotation = rotation;
}

bool meUCameraRotateObject::HasChanged() const{
	return ! pNewRotation.IsEqualTo( pOldRotation );
}

void meUCameraRotateObject::Undo(){
	pObject->SetRotation( pOldRotation );
	pObject->GetWorld()->NotifyObjectGeometryChanged( pObject );
}

void meUCameraRotateObject::Redo(){
	pObject->SetRotation( pNewRotation );
	pObject->GetWorld()->NotifyObjectGeometryChanged( pObject );
}

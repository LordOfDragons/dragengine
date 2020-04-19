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

#include "meUCameraMoveObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUCameraMoveObject
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUCameraMoveObject::meUCameraMoveObject( meObject *object ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	pOldPosition = object->GetPosition();
	pNewPosition = pOldPosition;
	SetShortInfo( "Move Camera Object" );
	
	pObject = object;
	object->AddReference();
}

meUCameraMoveObject::~meUCameraMoveObject(){
	if( pObject ) pObject->FreeReference();
}



// Undo and Redo operations
/////////////////////////////

void meUCameraMoveObject::SetNewPosition( const decDVector &position ){
	pNewPosition = position;
}

bool meUCameraMoveObject::HasChanged() const{
	return ! pNewPosition.IsEqualTo( pOldPosition );
}

void meUCameraMoveObject::Undo(){
	pObject->SetPosition( pOldPosition );
	pObject->GetWorld()->NotifyObjectGeometryChanged( pObject );
}

void meUCameraMoveObject::Redo(){
	pObject->SetPosition( pNewPosition );
	pObject->GetWorld()->NotifyObjectGeometryChanged( pObject );
}

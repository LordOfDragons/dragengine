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

#include "meUSetObjectPosition.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUSetObjectPosition
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUSetObjectPosition::meUSetObjectPosition( meObject *object, const decDVector &newPosition ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	meWorld *world = object->GetWorld();
	if( ! world ) DETHROW( deeInvalidParam );
	
	pOldPosition = object->GetPosition();
	pNewPosition = newPosition;
	
	SetShortInfo( "Set Object position" );
	
	pObject = object;
	object->AddReference();
}

meUSetObjectPosition::~meUSetObjectPosition(){
	if( pObject ) pObject->FreeReference();
}



// Management
///////////////

void meUSetObjectPosition::Undo(){
	meWorld *world = pObject->GetWorld();
	
	pObject->SetPosition( pOldPosition );
	
	world->NotifyObjectGeometryChanged( pObject );
}

void meUSetObjectPosition::Redo(){
	meWorld *world = pObject->GetWorld();
	
	pObject->SetPosition( pNewPosition );
	
	world->NotifyObjectGeometryChanged( pObject );
}

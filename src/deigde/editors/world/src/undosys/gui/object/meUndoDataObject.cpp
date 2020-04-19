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

#include "meUndoDataObject.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUndoDataObject
///////////////////////////

// Constructor, destructor
////////////////////////////

meUndoDataObject::meUndoDataObject( meObject *object ) :
pObject( NULL ),
pAttachedTo( NULL )
{
	if( ! object ){
		DETHROW( deeInvalidParam );
	}
	
	pOldPosition = object->GetPosition();
	pOldOrientation = object->GetRotation();
	pOldSize = object->GetSize();
	
	pObject = object;
	object->AddReference();
	
	pAttachedTo = object->GetAttachedTo();
	if( pAttachedTo ){
		pAttachedTo->AddReference();
	}
}

meUndoDataObject::~meUndoDataObject(){
	if( pAttachedTo ){
		pAttachedTo->FreeReference();
	}
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void meUndoDataObject::SetAttachedTo( meObject *object ){
	if( object == pAttachedTo ){
		return;
	}
	
	if( pAttachedTo ){
		pAttachedTo->FreeReference();
	}
	
	pAttachedTo = object;
	
	if( object ){
		object->AddReference();
	}
}

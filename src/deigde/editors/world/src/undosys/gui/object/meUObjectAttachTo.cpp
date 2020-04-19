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

#include "meUObjectAttachTo.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectAttachTo
////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectAttachTo::meUObjectAttachTo( meWorld *world, const meObjectList &objects, meObject *attachTo ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = objects.GetCount();
	meObject *object;
	decString text;
	
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pWorld = NULL;
	pAttachTo = NULL;
	pObjects = NULL;
	pObjectCount = 0;
	
	if( attachTo ){
		SetShortInfo( "Attach Objects To" );
		
	}else{
		SetShortInfo( "Detach Objects" );
	}
	
	if( count > 1 ){
		text.Format( "%i objects", count );
		
	}else{
		text = "1 object";
	}
	SetLongInfo( text.GetString() );
	
	try{
		pObjects = new sObject[ count ];
		
		for( pObjectCount=0; pObjectCount<count; pObjectCount++ ){
			object = objects.GetAt( pObjectCount );
			
			pObjects[ pObjectCount ].object = object;
			object->AddReference();
			pObjects[ pObjectCount ].oldAttachTo = object->GetAttachedTo();
			if( pObjects[ pObjectCount ].oldAttachTo ){
				pObjects[ pObjectCount ].oldAttachTo->AddReference();
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pAttachTo = attachTo;
	if( attachTo ){
		attachTo->AddReference();
	}
	
	pWorld = world;
	world->AddReference();
}

meUObjectAttachTo::~meUObjectAttachTo(){
	pCleanUp();
}



// Management
///////////////

void meUObjectAttachTo::Undo(){
	meObject *object;
	int o;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ].object;
		
		if( object != pObjects[ o ].oldAttachTo ){
			object->SetAttachedTo( pObjects[ o ].oldAttachTo );
			pWorld->NotifyObjectGeometryChanged( object );
		}
	}
}

void meUObjectAttachTo::Redo(){
	meObject *object;
	int o;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ].object;
		
		if( object != pAttachTo ){
			object->SetAttachedTo( pAttachTo );
			pWorld->NotifyObjectGeometryChanged( object );
		}
	}
}



// Private Functions
//////////////////////

void meUObjectAttachTo::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			pObjectCount--;
			if( pObjects[ pObjectCount ].oldAttachTo ){
				pObjects[ pObjectCount ].oldAttachTo->FreeReference();
			}
			pObjects[ pObjectCount ].object->FreeReference();
		}
		
		delete [] pObjects;
	}
	
	if( pAttachTo ){
		pAttachTo->FreeReference();
	}
	
	if( pWorld ){
		pWorld->FreeReference();
	}
}

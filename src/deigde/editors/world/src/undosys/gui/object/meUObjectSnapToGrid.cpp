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

#include "meUObjectSnapToGrid.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectSnapToGrid
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectSnapToGrid::meUObjectSnapToGrid( meWorld *world, const meObjectList &objects, float grid ) :
pWorld( NULL ),
pObjects( NULL ),
pObjectCount( 0 ),
pGrid( ( double )grid )
{
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = objects.GetCount();
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Snap objects to grid" );
	
	try{
		pObjects = new sObject[ count ];
		
		for( pObjectCount=0; pObjectCount<count; pObjectCount++ ){
			meObject * const object = objects.GetAt( pObjectCount );
			
			pObjects[ pObjectCount ].object = object;
			pObjects[ pObjectCount ].position = object->GetPosition();
			object->AddReference();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUObjectSnapToGrid::~meUObjectSnapToGrid(){
	pCleanUp();
}



// Management
///////////////

void meUObjectSnapToGrid::Undo(){
	int i;
	
	for( i=0; i<pObjectCount; i++ ){
		pObjects[ i ].object->SetPosition( pObjects[ i ].position );
		pWorld->NotifyObjectGeometryChanged( pObjects[ i ].object );
	}
}

void meUObjectSnapToGrid::Redo(){
	int i;
	
	for( i =0; i <pObjectCount; i++ ){
		decDVector position( pObjects[ i ].position );
		position.Snap( pGrid );
		pObjects[ i ].object->SetPosition( position );
		pWorld->NotifyObjectGeometryChanged( pObjects[ i ].object );
	}
}



// Private Functions
//////////////////////

void meUObjectSnapToGrid::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			pObjectCount--;
			pObjects[ pObjectCount ].object->FreeReference();
		}
		
		delete [] pObjects;
	}
	
	if( pWorld ){
		pWorld->FreeReference();
	}
}

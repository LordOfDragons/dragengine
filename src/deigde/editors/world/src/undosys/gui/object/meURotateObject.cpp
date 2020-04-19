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

#include "meURotateObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "dragengine/common/exceptions.h"



// Class meURotateObject
//////////////////////////

// Constructor, destructor
////////////////////////////

meURotateObject::meURotateObject( meWorld *world, const meObjectList &objects ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = objects.GetCount();
	
	SetShortInfo( "Rotate Object" );
	
	pWorld = NULL;
	
	pObjects = NULL;
	pObjectCount = 0;
	
	try{
		pWorld = world;
		world->AddReference();
		
		if( count > 0 ){
			pObjects = new meUndoDataObject*[ count ];
			
			while( pObjectCount < count ){
				pObjects[ pObjectCount ] = new meUndoDataObject( objects.GetAt( pObjectCount ) );
				pObjectCount++;
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meURotateObject::~meURotateObject(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void meURotateObject::Undo(){
	meObject *object;
	int i;
	
	for( i=0; i<pObjectCount; i++ ){
		object = pObjects[ i ]->GetObject();
		object->SetPosition( pObjects[ i ]->GetOldPosition() );
		object->SetRotation( pObjects[ i ]->GetOldOrientation() );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meURotateObject::Redo(){
	bool modifyPosition = GetModifyPosition();
	bool modifyOrientation = GetModifyOrientation();
	decDVector position, rotation;
	meObject *object;
	int o;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ]->GetObject();
		
		position = pObjects[ o ]->GetOldPosition();
		rotation = pObjects[ o ]->GetOldOrientation();
		
		TransformElement( position, rotation );
		
		object->SetRotation( modifyOrientation ? rotation.ToVector() : pObjects[ o ]->GetOldOrientation() );
		object->SetPosition( modifyPosition ? position : pObjects[ o ]->GetOldPosition() );
		
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meURotateObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meURotateObject::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			pObjectCount--;
			delete pObjects[ pObjectCount ];
		}
		
		delete [] pObjects;
	}
	
	if( pWorld ) pWorld->FreeReference();
}

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
	deObjectReference ref;
	
	SetShortInfo( "Rotate Object" );
	
	pWorld = NULL;
	
	try{
		pWorld = world;
		world->AddReference();
		
		int i;
		for( i=0; i<count; i++ ){
			ref.TakeOver( new meUndoDataObject( objects.GetAt( i ) ) );
			pObjects.Add( ref );
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
	const int count = pObjects.GetCount();
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		object = data.GetObject();
		object->SetPosition( data.GetOldPosition() );
		object->SetRotation( data.GetOldOrientation() );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meURotateObject::Redo(){
	bool modifyPosition = GetModifyPosition();
	bool modifyOrientation = GetModifyOrientation();
	const int count = pObjects.GetCount();
	decDVector position, rotation;
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		object = data.GetObject();
		
		position = data.GetOldPosition();
		rotation = data.GetOldOrientation();
		
		TransformElement( position, rotation );
		
		object->SetRotation( modifyOrientation ? rotation.ToVector() : data.GetOldOrientation() );
		object->SetPosition( modifyPosition ? position : data.GetOldPosition() );
		
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meURotateObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meURotateObject::pCleanUp(){
	if( pWorld ) pWorld->FreeReference();
}

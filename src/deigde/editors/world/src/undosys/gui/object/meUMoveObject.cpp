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

#include "meUMoveObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "../../../worldedit.h"
#include "dragengine/common/exceptions.h"



// Class meUMoveObject
////////////////////////

// Constructor, destructor
////////////////////////////

meUMoveObject::meUMoveObject( meWorld *world, const meObjectList &objects ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = objects.GetCount();
	deObjectReference ref;
	
	SetShortInfo( "Move Object" );
	
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

meUMoveObject::~meUMoveObject(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void meUMoveObject::Undo(){
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		object->SetPosition( data.GetOldPosition() );
		object->SetRotation( data.GetOldOrientation() );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meUMoveObject::Redo(){
	const int count = pObjects.GetCount();
	int i;
	
	if( GetModifyOrientation() ){
		
		for( i=0; i<count; i++ ){
			const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
			meObject * const object = data.GetObject();
			
			decDVector position( data.GetOldPosition() );
			decDVector rotation( data.GetOldOrientation() );
			
			TransformElement( position, rotation );
			
			object->SetRotation( rotation.ToVector() );
			object->SetPosition( position );
			pWorld->NotifyObjectGeometryChanged( object );
		}
		
	}else{
		const decDVector &distance = GetDistance();
		
		for( i=0; i<count; i++ ){
			const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
			meObject * const object = data.GetObject();
			object->SetPosition( data.GetOldPosition() + distance );
			object->SetRotation( data.GetOldOrientation() );
			pWorld->NotifyObjectGeometryChanged( object );
		}
	}
}

void meUMoveObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meUMoveObject::pCleanUp(){
	if( pWorld ){
		pWorld->FreeReference();
	}
}

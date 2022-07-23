/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "meUObjectCopyRotation.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCopyRotation
///////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCopyRotation::meUObjectCopyRotation( meWorld *world, bool copyX, bool copyY, bool copyZ ) :
pCopyX( copyX ),
pCopyY( copyY ),
pCopyZ( copyZ )
{
	if( ! world || ! world->GetSelectionObject().GetActive() ){
		DETHROW( deeInvalidParam );
	}
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	int i;
	
	SetShortInfo( "Copy Object Rotation" );
	SetLongInfo( "Copy Object Rotation" );
	
	for( i=0; i<count; i++ ){
		pObjects.Add( meUndoDataObject::Ref::New( new meUndoDataObject( list.GetAt( i ) ) ) );
	}
	
	pNewRotation = world->GetSelectionObject().GetActive()->GetRotation();
}

meUObjectCopyRotation::~meUObjectCopyRotation(){
}



// Management
///////////////

void meUObjectCopyRotation::Undo(){
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		object->SetRotation( data.GetOldOrientation() );
		object->GetWorld()->NotifyObjectGeometryChanged( object );
	}
}

void meUObjectCopyRotation::Redo(){
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		decVector rotation( data.GetOldOrientation() );
		
		if( pCopyX ){
			rotation.x = pNewRotation.x;
		}
		if( pCopyY ){
			rotation.y = pNewRotation.y;
		}
		if( pCopyZ ){
			rotation.z = pNewRotation.z;
		}
		
		object->SetRotation( rotation );
		object->GetWorld()->NotifyObjectGeometryChanged( object );
	}
}

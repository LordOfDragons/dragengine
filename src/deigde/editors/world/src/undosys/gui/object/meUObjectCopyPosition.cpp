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

#include "meUObjectCopyPosition.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectCopyPosition
////////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectCopyPosition::meUObjectCopyPosition( meWorld *world, bool copyX, bool copyY, bool copyZ ) :
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
	
	SetShortInfo( "Copy Object Position" );
	SetLongInfo( "Copy Object Position" );
	
	for( i=0; i<count; i++ ){
		pObjects.Add( meUndoDataObject::Ref::New( new meUndoDataObject( list.GetAt( i ) ) ) );
	}
	
	pNewPosition = world->GetSelectionObject().GetActive()->GetPosition();
}

meUObjectCopyPosition::~meUObjectCopyPosition(){
}



// Management
///////////////

void meUObjectCopyPosition::Undo(){
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		object->SetPosition( data.GetOldPosition() );
		object->GetWorld()->NotifyObjectGeometryChanged( object );
	}
}

void meUObjectCopyPosition::Redo(){
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		decDVector position( data.GetOldPosition() );
		
		if( pCopyX ){
			position.x = pNewPosition.x;
		}
		if( pCopyY ){
			position.y = pNewPosition.y;
		}
		if( pCopyZ ){
			position.z = pNewPosition.z;
		}
		
		object->SetPosition( position );
		object->GetWorld()->NotifyObjectGeometryChanged( object );
	}
}

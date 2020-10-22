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

#include "meUDeleteObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUDeleteObject
//////////////////////////

// Constructor, destructor
////////////////////////////

meUDeleteObject::meUDeleteObject( meWorld *world ) :
pWorld( NULL )
{
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	deObjectReference ref;
	decString text;
	int i;
	
	SetShortInfo( "Delete Objects" );
	if( count > 1 ){
		text.Format( "%i objects", count );
		
	}else{
		text = "1 object";
	}
	SetLongInfo( text );
	
	try{
		pWorld = world;
		world->AddReference();
		
		for( i=0; i<count; i++ ){
			ref.TakeOver( new meUndoDataObject( list.GetAt( i ) ) );
			pObjects.Add( ref );
		}
		
		// keep track of object attached to deleted objects
		const meObjectList &allObjects = world->GetObjects();
		const int allCount = allObjects.GetCount();
		for( i=0; i<allCount; i++ ){
			meObject * const object = allObjects.GetAt( i );
			if( object->GetAttachedTo() && list.Has( object->GetAttachedTo() ) ){
				ref.TakeOver( new meUndoDataObject( object ) );
				pAttached.Add( ref );
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUDeleteObject::~meUDeleteObject(){
	pCleanUp();
}



// Management
///////////////

void meUDeleteObject::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	const int count = pObjects.GetCount();
	int i;
	
	selection.Reset();
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		pWorld->AddObject( object );
		selection.Add( object );
		
		pWorld->NotifyObjectAdded( object );
		
		object->SetAttachedTo( data.GetAttachedTo() );
	}
	
	const int attachCount = pAttached.GetCount();
	for( i=0; i<attachCount; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pAttached.GetAt( i ) );
		meObject * const object = data.GetObject();
		object->SetAttachedTo( data.GetAttachedTo() );
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUDeleteObject::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	int i;
	
	const int attachCount = pAttached.GetCount();
	for( i=0; i<attachCount; i++ ){
		( ( meUndoDataObject* )pAttached.GetAt( i ) )->GetObject()->SetAttachedTo( NULL );
	}
	
	const int count = pObjects.GetCount();
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		object->SetAttachedTo( NULL );
			// done automatically if removed from world but better be safe
		
		selection.Remove( object );
		if( object->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject( object );
		
		pWorld->NotifyObjectRemoved( object );
	}
	
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUDeleteObject::pCleanUp(){
	if( pWorld ){
		pWorld->FreeReference();
	}
}

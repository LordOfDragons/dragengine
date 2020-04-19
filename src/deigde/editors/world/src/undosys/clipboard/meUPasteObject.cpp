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

#include <stdlib.h>

#include "meUPasteObject.h"
#include "../../world/meWorld.h"
#include "../../world/object/meObject.h"
#include "../../world/object/meObjectSelection.h"
#include "../../clipboard/meCDOObject.h"
#include "../../clipboard/meClipboardDataObject.h"
#include "../../worldedit.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class meUPasteObject
/////////////////////////

// Constructor, destructor
////////////////////////////

meUPasteObject::meUPasteObject( meWorld *world, meClipboardDataObject *clip ){
	if( ! world || ! clip ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = clip->GetObjectCount();
	meObject *object = NULL;
	int i;
	
	SetShortInfo( "Paste Objects" );
	SetLongInfo( "" );
	
	pWorld = NULL;
	
	if( count == 0 ){
		return;
	}
	
	pWorld = world;
	world->AddReference();
	
	try{
		for( i=0; i<count; i++ ){
			object = new meObject( world->GetEnvironment() );
			object->SetID( world->NextObjectID() );
			clip->GetObjectAt( i )->UpdateObject( *object );
			pObjects.Add( object );
			object->FreeReference();
			object = NULL;
		}
		
		for( i=0; i<count; i++ ){
			pAttachedToIndexList.Add( clip->GetObjectAt( i )->GetAttachToIndex() );
		}
		
	}catch( const deException & ){
		if( object ){
			object->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

meUPasteObject::~meUPasteObject(){
	pCleanUp();
}



// Management
///////////////

void meUPasteObject::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		meObject * const object = pObjects.GetAt( i );
		
		object->SetAttachedTo( NULL );
		
		selection.Remove( object );
		if( object->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject( object );
		
		pWorld->NotifyObjectRemoved( object );
	}
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUPasteObject::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	const int count = pObjects.GetCount();
	int i;
	
	selection.Reset();
	
	for( i=0; i<count; i++ ){
		meObject * const object = pObjects.GetAt( i );
		
		pWorld->AddObject( object );
		selection.Add( object );
		
		pWorld->NotifyObjectAdded( object );
	}
	
	for( i=0; i<count; i++ ){
		meObject * const object = pObjects.GetAt( i );
		
		const int attachToIndex = pAttachedToIndexList.GetAt( i );
		if( attachToIndex != -1 ){
			object->SetAttachedTo( pObjects.GetAt( attachToIndex ) );
			
		}else if( ! object->GetAttachedToID().IsEmpty() ){
			object->SetAttachedTo( pWorld->GetObjectWithID( object->GetAttachedToID() ) );
		}
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUPasteObject::pCleanUp(){
	if( pWorld ){
		pWorld->FreeReference();
	}
}

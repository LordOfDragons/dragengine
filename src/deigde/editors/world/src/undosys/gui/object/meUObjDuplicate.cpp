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
#include <unistd.h>

#include "meUObjDuplicate.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "../../../world/object/texture/meObjectTexture.h"

#include <dragengine/common/exceptions.h>



// Class meUObjDuplicate
//////////////////////////

// Constructor, destructor
////////////////////////////

meUObjDuplicate::meUObjDuplicate( meWorld *world, const decVector &offset ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const meObjectList &list = world->GetSelectionObject().GetSelected();
	const int count = list.GetCount();
	if( count == 0 ){
		DETHROW( deeInvalidParam );
	}
	
	deObjectReference ref, refObj, refTex;
	int i, j, textureCount;
	
	SetShortInfo( "Duplicate Objects" );
	
	pWorld = NULL;
	
	try{
		for( i=0; i<count; i++ ){
			const meObject &object = *list.GetAt( i );
			
			refObj.TakeOver( new meObject( world->GetEnvironment() ) );
			meObject * const duplicate = ( meObject* )( deObject* )refObj;
			
			duplicate->SetClassName( object.GetClassName() );
			duplicate->SetPosition( object.GetPosition() + decDVector( offset ) );
			duplicate->SetRotation( object.GetRotation() );
			duplicate->SetSize( object.GetSize() );
			duplicate->SetScaling( object.GetScaling() );
			duplicate->SetProperties( object.GetProperties() );
			
			textureCount = object.GetTextureCount();
			for( j=0; j<textureCount; j++ ){
				refTex.TakeOver( new meObjectTexture( *object.GetTextureAt( j ) ) );
				duplicate->AddTexture( ( meObjectTexture* )( deObject* )refTex );
			}
			
			duplicate->SetID( world->NextObjectID() );
			
			ref.TakeOver( new meUndoDataObject( duplicate ) );
			pObjects.Add( ref );
		}
		
		// update attachment
		for( i=0; i<count; i++ ){
			const meObject &object = *list.GetAt( i );
			if( ! object.GetAttachedTo() ){
				continue;
			}
			
			meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
			for( j=0; j<count; j++ ){
				if( list.GetAt( j ) == object.GetAttachedTo() ){
					// attached to a duplicated object
					data.SetAttachedTo( ( ( meUndoDataObject* )pObjects.GetAt( j ) )->GetObject() );
					break;
				}
			}
			
			// attached to a not duplicated object
			if( ! data.GetAttachedTo() ){
				data.SetAttachedTo( object.GetAttachedTo() );
			}
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	pWorld = world;
	world->AddReference();
}

meUObjDuplicate::~meUObjDuplicate(){
	pCleanUp();
}



// Management
///////////////

void meUObjDuplicate::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	const int count = pObjects.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		meObject * const object = data.GetObject();
		
		object->SetAttachedTo( NULL );
		
		selection.Remove( object );
		if( object->GetActive() ){
			selection.ActivateNext();
		}
		
		pWorld->RemoveObject( data.GetObject() );
		
		pWorld->NotifyObjectRemoved( object );
	}
	
	pWorld->NotifyObjectSelectionChanged();
}

void meUObjDuplicate::Redo(){
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
	}
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		data.GetObject()->SetAttachedTo( data.GetAttachedTo() );
	}
	
	selection.ActivateNext();
	
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUObjDuplicate::pCleanUp(){
	if( pWorld ) pWorld->FreeReference();
}

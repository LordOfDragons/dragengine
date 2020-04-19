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

#include "meUAddObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <dragengine/common/exceptions.h>



// Class meUAddObject
///////////////////////

// Constructor, destructor
////////////////////////////

meUAddObject::meUAddObject( meWorld *world, meObject *object ){
	if( ! world || ! object ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add Object" );
	
	pWorld = world;
	world->AddReference();
	
	pObject = object;
	object->AddReference();
}

meUAddObject::meUAddObject( meWorld *world, const decDVector &position, const char *classname ){
	if( ! world || ! classname ){
		DETHROW( deeInvalidParam );
	}
	
	decString text;
	
	SetShortInfo( "Add Object" );
	text.Format( "position(%g,%g,%g) class(%s)", position.x, position.y, position.z, classname );
	SetLongInfo( text.GetString() );
	
	pWorld = NULL;
	pObject = NULL;
	
	try{
		pWorld = world;
		world->AddReference();
		
		pObject = new meObject( world->GetEnvironment() );
		pObject->SetPosition( position );
		pObject->SetSize( decVector( 0.5f, 0.5f, 0.5f ) );
		pObject->SetClassName( classname );
		pObject->SetID( world->NextObjectID() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUAddObject::~meUAddObject(){
	pCleanUp();
}



// Management
///////////////

void meUAddObject::Undo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	selection.Remove( pObject );
	if( pObject->GetActive() ){
		selection.ActivateNext();
	}
	
	pWorld->RemoveObject( pObject );
	
	pWorld->NotifyObjectRemoved( pObject );
	pWorld->NotifyObjectSelectionChanged();
}

void meUAddObject::Redo(){
	meObjectSelection &selection = pWorld->GetSelectionObject();
	
	pWorld->AddObject( pObject );
	
	selection.Reset();
	selection.Add( pObject );
	selection.SetActive( pObject );
	
	pWorld->NotifyObjectAdded( pObject );
	pWorld->NotifyObjectSelectionChanged();
}



// Private Functions
//////////////////////

void meUAddObject::pCleanUp(){
	if( pObject ){
		pObject->FreeReference();
	}
	if( pWorld ){
		pWorld->FreeReference();
	}
}

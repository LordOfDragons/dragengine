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

#include "meUSetObjectClass.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectList.h"

#include <dragengine/common/exceptions.h>



// Class meUSetObjectClass
////////////////////////////

// Constructor, destructor
////////////////////////////

meUSetObjectClass::meUSetObjectClass( meObject *object, const char *newcname ){
	if( ! object || ! newcname ) DETHROW( deeInvalidParam );
	
	meWorld *world = object->GetWorld();
	if( ! world ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Set Object class" );
	
	pObjects = NULL;
	pObjectCount = 0;
	
	try{
		pObjects = new sObject[ 1 ];
		if( ! pObjects ) DETHROW( deeOutOfMemory );
		
		pObjects[ 0 ].oldcname = object->GetClassName();
		pObjects[ 0 ].newcname = newcname;
		pObjects[ 0 ].oldsize = object->GetSize();
		pObjects[ 0 ].oldscaling = object->GetScaling();
		pObjects[ 0 ].object = object;
		object->AddReference();
		
		pObjectCount = 1;
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUSetObjectClass::meUSetObjectClass( meObjectList &objects, const char *newcname ){
	int count = objects.GetCount();
	meObject *object;
	
	if( count == 0 || ! newcname ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Set Object classes" );
	
	pObjects = NULL;
	pObjectCount = 0;
	
	try{
		pObjects = new sObject[ count ];
		if( ! pObjects ) DETHROW( deeOutOfMemory );
		
		for( pObjectCount=0; pObjectCount<count; pObjectCount++ ){
			object = objects.GetAt( pObjectCount );
			
			pObjects[ pObjectCount ].oldcname = object->GetClassName();
			pObjects[ pObjectCount ].newcname = newcname;
			pObjects[ pObjectCount ].oldsize = object->GetSize();
			pObjects[ pObjectCount ].oldscaling = object->GetScaling();
			pObjects[ pObjectCount ].object = object;
			object->AddReference();
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

meUSetObjectClass::~meUSetObjectClass(){
	pCleanUp();
}



// Management
///////////////

void meUSetObjectClass::Undo(){
	meObject *object;
	meWorld *world;
	int o;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ].object;
		world = object->GetWorld();
		
		object->SetClassName( pObjects[ o ].oldcname.GetString() );
		object->SetSizeAndScaling( pObjects[ o ].oldsize, pObjects[ o ].oldscaling );
		
		world->NotifyObjectClassChanged( object );
		world->NotifyObjectGeometryChanged( object );
	}
}

void meUSetObjectClass::Redo(){
	meObject *object;
	meWorld *world;
	int o;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ].object;
		world = object->GetWorld();
		
		object->SetClassName( pObjects[ o ].newcname.GetString() );
		
		world->NotifyObjectClassChanged( object );
		world->NotifyObjectGeometryChanged( object );
	}
}



// Private Functions
//////////////////////

void meUSetObjectClass::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			pObjectCount--;
			pObjects[ pObjectCount ].object->FreeReference();
		}
		
		delete [] pObjects;
	}
}

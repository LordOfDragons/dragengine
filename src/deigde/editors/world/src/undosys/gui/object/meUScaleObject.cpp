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

#include "meUScaleObject.h"
#include "meUndoDataObject.h"

#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"

#include <deigde/gamedefinition/class/igdeGDClass.h>

#include <dragengine/common/exceptions.h>



// Class meUScaleObject
/////////////////////////

// Constructor, destructor
////////////////////////////

meUScaleObject::meUScaleObject( meWorld *world, const meObjectList &objects ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = objects.GetCount();
	
	SetShortInfo( "Scale Object" );
	
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

meUScaleObject::~meUScaleObject(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void meUScaleObject::Undo(){
	meObject *object;
	int i;
	
	for( i=0; i<pObjectCount; i++ ){
		object = pObjects[ i ]->GetObject();
		object->SetPosition( pObjects[ i ]->GetOldPosition() );
		object->SetSize( pObjects[ i ]->GetOldSize() );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meUScaleObject::Redo(){
	const decVector &factors = GetFactors();
	const decDVector &pivot = GetPivot();
	bool modifyPosition = GetModifyPosition();
	float uniformFactor = GetUniformFactor();
	bool modifySize = GetModifySize();
	decDVector position;
	meObject *object;
	int o, scaleMode;
	
	for( o=0; o<pObjectCount; o++ ){
		object = pObjects[ o ]->GetObject();
		
		if( modifySize ){
			const decVector &size = pObjects[ o ]->GetOldSize();
			scaleMode = object->GetScaleMode();
			
			if( scaleMode == igdeGDClass::esmUniform ){
				object->SetSize( size * uniformFactor );
				
			}else if( scaleMode == igdeGDClass::esmFree ){
				object->SetSize( decVector( size.x * factors.x, size.y * factors.y, size.z * factors.z ) );
			}
		}
		
		if( modifyPosition ){
			position = pObjects[ o ]->GetOldPosition() - pivot;
			position.x = pivot.x + position.x * ( double )factors.x;
			position.y = pivot.y + position.y * ( double )factors.y;
			position.z = pivot.z + position.z * ( double )factors.z;
			object->SetPosition( position );
		}
		
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meUScaleObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meUScaleObject::pCleanUp(){
	if( pObjects ){
		while( pObjectCount > 0 ){
			pObjectCount--;
			delete pObjects[ pObjectCount ];
		}
		
		delete [] pObjects;
	}
	
	if( pWorld ) pWorld->FreeReference();
}

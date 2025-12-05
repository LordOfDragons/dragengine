/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	
	try{
		pWorld = world;
		world->AddReference();
		
		int i;
		for( i=0; i<count; i++ ){
			pObjects.Add(meUndoDataObject::Ref::NewWith(objects.GetAt(i)));
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
	const int count = pObjects.GetCount();
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		object = data.GetObject();
		object->SetPosition( data.GetOldPosition() );
		object->SetSize( data.GetOldSize() );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meUScaleObject::Redo(){
	const int count = pObjects.GetCount();
	const decVector &factors = GetFactors();
	const decDVector &pivot = GetPivot();
	bool modifyPosition = GetModifyPosition();
	float uniformFactor = GetUniformFactor();
	bool modifySize = GetModifySize();
	decDVector position;
	meObject *object;
	int i, scaleMode;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		object = data.GetObject();
		
		if( modifySize ){
			const decVector &size = data.GetOldSize();
			scaleMode = object->GetScaleMode();
			
			if( scaleMode == igdeGDClass::esmUniform ){
				object->SetSize( size * uniformFactor );
				
			}else if( scaleMode == igdeGDClass::esmFree ){
				object->SetSize( decVector( size.x * factors.x, size.y * factors.y, size.z * factors.z ) );
			}
		}
		
		if( modifyPosition ){
			position = data.GetOldPosition() - pivot;
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
	if( pWorld ) pWorld->FreeReference();
}

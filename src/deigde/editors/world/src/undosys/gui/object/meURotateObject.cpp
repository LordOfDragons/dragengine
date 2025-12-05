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

#include "meURotateObject.h"
#include "meUndoDataObject.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"
#include "../../../world/object/meObjectSelection.h"
#include "dragengine/common/exceptions.h"



// Class meURotateObject
//////////////////////////

// Constructor, destructor
////////////////////////////

meURotateObject::meURotateObject( meWorld *world, const meObjectList &objects ){
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = objects.GetCount();
	
	SetShortInfo( "Rotate Object" );
	
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

meURotateObject::~meURotateObject(){
	pCleanUp();
}



// Undo and Redo operations
/////////////////////////////

void meURotateObject::Undo(){
	const int count = pObjects.GetCount();
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		object = data.GetObject();
		object->SetPosition( data.GetOldPosition() );
		object->SetRotation( data.GetOldOrientation() );
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meURotateObject::Redo(){
	bool modifyPosition = GetModifyPosition();
	bool modifyOrientation = GetModifyOrientation();
	const int count = pObjects.GetCount();
	decDVector position, rotation;
	meObject *object;
	int i;
	
	for( i=0; i<count; i++ ){
		const meUndoDataObject &data = *( ( meUndoDataObject* )pObjects.GetAt( i ) );
		object = data.GetObject();
		
		position = data.GetOldPosition();
		rotation = data.GetOldOrientation();
		
		TransformElement( position, rotation );
		
		object->SetRotation( modifyOrientation ? rotation.ToVector() : data.GetOldOrientation() );
		object->SetPosition( modifyPosition ? position : data.GetOldPosition() );
		
		pWorld->NotifyObjectGeometryChanged( object );
	}
}

void meURotateObject::ProgressiveRedo(){
	Redo(); // redo is enough in this situation
}



// Private Functions
//////////////////////

void meURotateObject::pCleanUp(){
	if( pWorld ) pWorld->FreeReference();
}

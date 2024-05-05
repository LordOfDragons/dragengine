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

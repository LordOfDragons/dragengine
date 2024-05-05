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

#include "meUSetObjectSize.h"
#include "../../../world/meWorld.h"
#include "../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUSetObjectSize
///////////////////////////

// Constructor, destructor
////////////////////////////

meUSetObjectSize::meUSetObjectSize( meObject *object, const decVector &newSize ){
	if( ! object ) DETHROW( deeInvalidParam );
	
	meWorld *world = object->GetWorld();
	if( ! world ) DETHROW( deeInvalidParam );
	
	pOldSize = object->GetSize();
	pNewSize = newSize;
	
	SetShortInfo( "Set Object size" );
	
	pObject = object;
	object->AddReference();
}

meUSetObjectSize::~meUSetObjectSize(){
	if( pObject ) pObject->FreeReference();
}



// Management
///////////////

void meUSetObjectSize::Undo(){
	meWorld *world = pObject->GetWorld();
	
	pObject->SetSize( pOldSize );
	
	world->NotifyObjectGeometryChanged( pObject );
}

void meUSetObjectSize::Redo(){
	meWorld *world = pObject->GetWorld();
	
	pObject->SetSize( pNewSize );
	
	world->NotifyObjectGeometryChanged( pObject );
}

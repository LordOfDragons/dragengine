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

#include <stddef.h>

#include "deObject.h"
#include "common/exceptions.h"



// Class deObjectReference
////////////////////////////

// Constructor, destructor
////////////////////////////

deObjectReference::deObjectReference() :
pObject( NULL ){
}

deObjectReference::deObjectReference( deObject *object ) :
pObject( object )
{
	if( pObject ){
		pObject->AddReference();
	}
}

deObjectReference::deObjectReference( const deObjectReference &reference ) :
pObject( reference.pObject )
{
	if( pObject ){
		pObject->AddReference();
	}
}

deObjectReference::~deObjectReference(){
	if( pObject ){
		pObject->FreeReference();
	}
}



// Management
///////////////

void deObjectReference::TakeOver( deObject *object ){
	if( object == pObject ){
		if( object ){
			// this is required since we are asked to take over the reference. since we
			// have the same reference already we refuse to take over the reference and
			// thus without releasing it this reference would be dangling
			object->FreeReference();
		}
		return;
	}
	
	if( pObject ){
		pObject->FreeReference();
	}
	pObject = object;
}

bool deObjectReference::operator!() const{
	return pObject == NULL;
}

deObjectReference::operator bool() const{
	return pObject != NULL;
}

deObjectReference::operator deObject &() const{
	if( ! pObject ){
		DETHROW( deeNullPointer );
	}
	return *pObject;
}

deObject *deObjectReference::operator->() const{
	if( ! pObject ){
		DETHROW( deeNullPointer );
	}
	return pObject;
}

deObjectReference &deObjectReference::operator=( deObject *object ){
	if( object == pObject ){
		return *this;
	}
	
	if( pObject ){
		pObject->FreeReference();
	}
	
	pObject = object;
	
	if( object ){
		object->AddReference();
	}
	
	return *this;
}

deObjectReference & deObjectReference::operator=( const deObjectReference &reference ){
	return operator=( reference.pObject );
}

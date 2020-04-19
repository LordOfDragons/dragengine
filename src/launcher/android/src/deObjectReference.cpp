/* 
 * Drag[en]gine Android Launcher
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

#include <stddef.h>

#include "deObject.h"
#include "deObjectReference.h"
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

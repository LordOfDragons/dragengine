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

#include "meUObjectSetProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectSetProperty
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectSetProperty::meUObjectSetProperty( meObject *object, const char *key, const char *oldValue, const char *newValue ){
	if( ! object || ! key || ! oldValue || ! newValue ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = object->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Change object property" );
	
	pObject = NULL;
	
	pKey = key;
	pOldValue = oldValue;
	pNewValue = newValue;
	
	pObject = object;
	object->AddReference();
}

meUObjectSetProperty::~meUObjectSetProperty(){
	pCleanUp();
}



// Management
///////////////

void meUObjectSetProperty::SetNewValue( const char *value ){
	pNewValue = value;
}



void meUObjectSetProperty::Undo(){
	pObject->SetProperty( pKey.GetString(), pOldValue.GetString() );
}

void meUObjectSetProperty::Redo(){
	pObject->SetProperty( pKey.GetString(), pNewValue.GetString() );
}

void meUObjectSetProperty::ProgressiveRedo(){
	Redo();
}



// Private Functions
//////////////////////

void meUObjectSetProperty::pCleanUp(){
	if( pObject ){
		pObject->FreeReference();
	}
}

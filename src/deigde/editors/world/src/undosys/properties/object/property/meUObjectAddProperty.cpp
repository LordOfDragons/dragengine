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

#include "meUObjectAddProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/object/meObject.h"

#include <dragengine/common/exceptions.h>



// Class meUObjectAddProperty
///////////////////////////////

// Constructor, destructor
////////////////////////////

meUObjectAddProperty::meUObjectAddProperty( meObject *object, const char *key, const char *value ){
	if( ! object || ! key || strlen( key ) == 0 || ! value ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = object->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add object property" );
	
	pObject = NULL;
	
	pKey = key;
	pValue = value;
	
	pObject = object;
	object->AddReference();
}

meUObjectAddProperty::~meUObjectAddProperty(){
	pCleanUp();
}



// Management
///////////////

void meUObjectAddProperty::SetValue( const char *value ){
	pValue = value;
}



void meUObjectAddProperty::Undo(){
	pObject->RemoveProperty( pKey.GetString() );
}

void meUObjectAddProperty::Redo(){
	pObject->SetProperty( pKey.GetString(), pValue.GetString() );
}

void meUObjectAddProperty::ProgressiveRedo(){
	Redo();
}



// Private Functions
//////////////////////

void meUObjectAddProperty::pCleanUp(){
	if( pObject ){
		pObject->FreeReference();
	}
}

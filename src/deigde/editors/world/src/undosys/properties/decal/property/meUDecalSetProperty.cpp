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

#include "meUDecalSetProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalSetProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalSetProperty::meUDecalSetProperty( meDecal *decal, const char *key, const char *oldValue, const char *newValue ) :
pDecal( NULL )
{
	if( ! decal || ! key || ! oldValue || ! newValue ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Change decal property" );
	
	pDecal = NULL;
	
	pKey = key;
	pOldValue = oldValue;
	pNewValue = newValue;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalSetProperty::~meUDecalSetProperty(){
	pCleanUp();
}



// Management
///////////////

void meUDecalSetProperty::SetNewValue( const char *value ){
	pNewValue = value;
}



void meUDecalSetProperty::Undo(){
	pDecal->SetProperty( pKey, pOldValue );
}

void meUDecalSetProperty::Redo(){
	pDecal->SetProperty( pKey, pNewValue );
}

void meUDecalSetProperty::ProgressiveRedo(){
	Redo();
}



// Private Functions
//////////////////////

void meUDecalSetProperty::pCleanUp(){
	if( pDecal ){
		pDecal->FreeReference();
	}
}

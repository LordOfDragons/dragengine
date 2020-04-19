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

#include "meUDecalAddProperty.h"
#include "../../../../world/meWorld.h"
#include "../../../../world/decal/meDecal.h"

#include <dragengine/common/exceptions.h>



// Class meUDecalAddProperty
//////////////////////////////

// Constructor, destructor
////////////////////////////

meUDecalAddProperty::meUDecalAddProperty( meDecal *decal, const char *key, const char *value ) :
pDecal( NULL )
{
	if( ! decal || ! key || strlen( key ) == 0 || ! value ){
		DETHROW( deeInvalidParam );
	}
	
	meWorld * const world = decal->GetWorld();
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Add decal property" );
	
	pDecal = NULL;
	
	pKey = key;
	pValue = value;
	
	pDecal = decal;
	decal->AddReference();
}

meUDecalAddProperty::~meUDecalAddProperty(){
	pCleanUp();
}



// Management
///////////////

void meUDecalAddProperty::SetValue( const char *value ){
	pValue = value;
}



void meUDecalAddProperty::Undo(){
	pDecal->RemoveProperty( pKey );
}

void meUDecalAddProperty::Redo(){
	pDecal->SetProperty( pKey, pValue );
}

void meUDecalAddProperty::ProgressiveRedo(){
	Redo();
}



// Private Functions
//////////////////////

void meUDecalAddProperty::pCleanUp(){
	if( pDecal ){
		pDecal->FreeReference();
	}
}

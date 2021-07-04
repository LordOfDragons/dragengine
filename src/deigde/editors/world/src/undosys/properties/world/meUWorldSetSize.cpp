/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "meUWorldSetSize.h"
#include "../../../world/meWorld.h"

#include <dragengine/common/exceptions.h>



// Class meUWorldSetSize
//////////////////////////

// Constructor, destructor
////////////////////////////

meUWorldSetSize::meUWorldSetSize( meWorld *world, const decDVector &newValue ) :
pWorld( NULL ),
pNewValue( newValue )
{
	if( ! world ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set world size" );
	
	pOldValue = world->GetSize();
	
	pWorld = world;
	world->AddReference();
}

meUWorldSetSize::~meUWorldSetSize(){
	if( pWorld ){
		pWorld->FreeReference();
	}
}



// Management
///////////////

void meUWorldSetSize::Undo(){
	pWorld->SetSize( pOldValue );
}

void meUWorldSetSize::Redo(){
	pWorld->SetSize( pNewValue );
}

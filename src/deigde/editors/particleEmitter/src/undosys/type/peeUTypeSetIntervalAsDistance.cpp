/* 
 * Drag[en]gine IGDE Particle Emitter Editor
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
#include <string.h>
#include <stdlib.h>

#include "peeUTypeSetIntervalAsDistance.h"
#include "../../emitter/peeType.h"

#include <dragengine/common/exceptions.h>



// Class peeUTypeSetIntervalAsDistance
////////////////////////////////////////

// Constructor, destructor
////////////////////////////

peeUTypeSetIntervalAsDistance::peeUTypeSetIntervalAsDistance( peeType *type ){
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	pType = NULL;
	
	SetShortInfo( "Set Type Interval As Distance" );
	
	pType = type;
	type->AddReference();
}

peeUTypeSetIntervalAsDistance::~peeUTypeSetIntervalAsDistance(){
	if( pType ){
		pType->FreeReference();
	}
}



// Management
///////////////

void peeUTypeSetIntervalAsDistance::Undo(){
	pType->SetIntervalAsDistance( ! pType->GetIntervalAsDistance() );
}

void peeUTypeSetIntervalAsDistance::Redo(){
	pType->SetIntervalAsDistance( ! pType->GetIntervalAsDistance() );
}

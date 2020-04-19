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

#include "peeUControllerSetUpper.h"
#include "../../emitter/peeController.h"

#include <dragengine/common/exceptions.h>



// Class peeUControllerSetUpper
/////////////////////////////////

// Constructor, destructor
////////////////////////////

peeUControllerSetUpper::peeUControllerSetUpper( peeController *controller, float newValue ) :
pController( NULL ),
pNewUpper( newValue )
{
	if( ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Controller Upper" );
	
	pOldUpper = controller->GetUpper();
	pOldLower = controller->GetLower();
	
	pController = controller;
	controller->AddReference();
}

peeUControllerSetUpper::~peeUControllerSetUpper(){
	if( pController ){
		pController->FreeReference();
	}
}



// Management
///////////////

void peeUControllerSetUpper::Undo(){
	pController->SetUpper( pOldUpper );
	pController->SetLower( pOldLower );
}

void peeUControllerSetUpper::Redo(){
	pController->SetUpper( pNewUpper );
}

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

#include "peeUTypeSetTrailController.h"
#include "../../emitter/peeType.h"

#include <dragengine/common/exceptions.h>



// Class peeUTypeSetTrailController
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

peeUTypeSetTrailController::peeUTypeSetTrailController( peeType *type,
deParticleEmitterType::eEmitControllers controller, const char *newTarget ) :
pType( NULL ),
pController( controller ),
pNewTarget( newTarget )
{
	if( ! type ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Set Type Trail Controller" );
	
	pOldTarget = type->GetTrailController( controller );
	
	pType = type;
	type->AddReference();
}

peeUTypeSetTrailController::~peeUTypeSetTrailController(){
	if( pType ){
		pType->FreeReference();
	}
}



// Management
///////////////

void peeUTypeSetTrailController::Undo(){
	pType->SetTrailController( pController, pOldTarget );
}

void peeUTypeSetTrailController::Redo(){
	pType->SetTrailController( pController, pNewTarget );
}

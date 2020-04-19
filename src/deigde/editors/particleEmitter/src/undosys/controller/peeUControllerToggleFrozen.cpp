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

#include "peeUControllerToggleFrozen.h"
#include "../../emitter/peeController.h"

#include <dragengine/common/exceptions.h>



// Class peeUControllerToggleFrozen
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

peeUControllerToggleFrozen::peeUControllerToggleFrozen( peeController *controller ){
	if( ! controller ) DETHROW( deeInvalidParam );
	
	pController = NULL;
	
	SetShortInfo( "Toggle Controller Frozen" );
	
	pController = controller;
	controller->AddReference();
}

peeUControllerToggleFrozen::~peeUControllerToggleFrozen(){
	if( pController ){
		pController->FreeReference();
	}
}



// Management
///////////////

void peeUControllerToggleFrozen::Undo(){
	pController->SetFrozen( ! pController->GetFrozen() );
}

void peeUControllerToggleFrozen::Redo(){
	pController->SetFrozen( ! pController->GetFrozen() );
}

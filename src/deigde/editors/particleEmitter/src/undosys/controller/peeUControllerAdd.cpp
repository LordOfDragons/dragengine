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

#include "peeUControllerAdd.h"
#include "../../emitter/peeEmitter.h"
#include "../../emitter/peeController.h"

#include <dragengine/common/exceptions.h>



// Class peeUControllerAdd
////////////////////////////

// Constructor, destructor
////////////////////////////

peeUControllerAdd::peeUControllerAdd( peeEmitter *emitter, peeController *controller ){
	if( ! emitter || ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	pEmitter = NULL;
	pController = NULL;
	
	SetShortInfo( "Add Controller" );
	
	pEmitter = emitter;
	emitter->AddReference();
	
	pController = controller;
	controller->AddReference();
}

peeUControllerAdd::~peeUControllerAdd(){
	if( pController ){
		pController->FreeReference();
	}
	if( pEmitter ){
		pEmitter->FreeReference();
	}
}



// Management
///////////////

void peeUControllerAdd::Undo(){
	pEmitter->RemoveController( pController );
}

void peeUControllerAdd::Redo(){
	pEmitter->AddController( pController );
	pEmitter->SetActiveController( pController );
}

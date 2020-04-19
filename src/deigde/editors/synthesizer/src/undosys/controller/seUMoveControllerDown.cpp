/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#include "seUMoveControllerDown.h"
#include "../../synthesizer/seSynthesizer.h"
#include "../../synthesizer/controller/seController.h"

#include <dragengine/common/exceptions.h>



// Class seUMoveControllerDown
////////////////////////////////

// Constructor, destructor
////////////////////////////

seUMoveControllerDown::seUMoveControllerDown( seSynthesizer *synthesizer, seController *controller ) :
pSynthesizer( NULL ),
pController( NULL )
{
	if( ! synthesizer || ! controller ){
		DETHROW( deeInvalidParam );
	}
	
	pIndex = synthesizer->GetControllers().IndexOf( controller );
	if( pIndex == -1 || pIndex == synthesizer->GetControllers().GetCount() - 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pSynthesizer = synthesizer;
	synthesizer->AddReference();
	
	pController = controller;
	controller->AddReference();
}

seUMoveControllerDown::~seUMoveControllerDown(){
	pCleanUp();
}



// Management
///////////////

void seUMoveControllerDown::Undo(){
	pSynthesizer->MoveControllerTo( pController, pIndex );
}

void seUMoveControllerDown::Redo(){
	pSynthesizer->MoveControllerTo( pController, pIndex + 1 );
}



// Private Functions
//////////////////////

void seUMoveControllerDown::pCleanUp(){
	if( pController ){
		pController->FreeReference();
	}
	if( pSynthesizer ){
		pSynthesizer->FreeReference();
	}
}

/* 
 * Drag[en]gine IGDE Animator Editor
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

#include "aeUControllerSetFromMove.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"

#include <dragengine/common/exceptions.h>



// Class aeUControllerSetFromMove
///////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUControllerSetFromMove::aeUControllerSetFromMove( aeController *controller,
	float newMinimumValue, float newMaximumValue ) :
pController( controller ),
pNewMinimumValue( newMinimumValue ),
pNewMaximumValue( newMaximumValue )
{
	if( ! controller || ! controller->GetAnimator() ){
		DETHROW( deeInvalidParam );
	}
	
	pOldMinimumValue = controller->GetMinimumValue();
	pOldMaximumValue = controller->GetMaximumValue();
	SetShortInfo( "Set controller from move" );
}

aeUControllerSetFromMove::~aeUControllerSetFromMove(){
}



// Management
///////////////

void aeUControllerSetFromMove::Undo(){
	aeController &controller = ( aeController& )( deObject& )pController;
	controller.SetMinimumValue( pOldMinimumValue );
	controller.SetMaximumValue( pOldMaximumValue );
	controller.SetCurrentValue( pOldMinimumValue );
}

void aeUControllerSetFromMove::Redo(){
	aeController &controller = ( aeController& )( deObject& )pController;
	controller.SetMinimumValue( pNewMinimumValue );
	controller.SetMaximumValue( pNewMaximumValue );
	controller.SetCurrentValue( pNewMinimumValue );
}

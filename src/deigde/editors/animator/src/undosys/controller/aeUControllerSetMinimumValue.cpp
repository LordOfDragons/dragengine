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

#include "aeUControllerSetMinimumValue.h"
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeController.h"

#include <dragengine/common/exceptions.h>



// Class aeUControllerSetMinimumValue
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

aeUControllerSetMinimumValue::aeUControllerSetMinimumValue( aeController *controller, float newValue ) :
pController( controller ),
pNewValue( newValue )
{
	if( ! controller || ! controller->GetAnimator() ){
		DETHROW( deeInvalidParam );
	}
	
	pOldValue = controller->GetMinimumValue();
	SetShortInfo( "Set controller minimum value" );
}

aeUControllerSetMinimumValue::~aeUControllerSetMinimumValue(){
}



// Management
///////////////

void aeUControllerSetMinimumValue::Undo(){
	( ( aeController& )( deObject& )pController ).SetMinimumValue( pOldValue );
}

void aeUControllerSetMinimumValue::Redo(){
	( ( aeController& )( deObject& )pController ).SetMinimumValue( pNewValue );
}

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

#include "peeUParameterSetControllerValue.h"
#include "../../emitter/peeType.h"
#include "../../emitter/peeParameter.h"
#include "../../emitter/peeController.h"

#include <dragengine/common/exceptions.h>



// Class peeUParameterSetControllerValue
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

peeUParameterSetControllerValue::peeUParameterSetControllerValue( peeType *type, peeParameter *parameter, peeController *newController ){
	if( ! type || ! parameter ){
		DETHROW( deeInvalidParam );
	}
	
	pType = NULL;
	pParameter = parameter;
	pOldController = NULL;
	pNewController = NULL;
	
	SetShortInfo( "Set Paramater Value Curve Controller" );
	
	pOldController = parameter->GetControllerValue();
	if( pOldController ){
		pOldController->AddReference();
	}
	
	pNewController = newController;
	if( pNewController ){
		pNewController->AddReference();
	}
	
	pType = type;
	type->AddReference();
}

peeUParameterSetControllerValue::~peeUParameterSetControllerValue(){
	if( pNewController ){
		pNewController->FreeReference();
	}
	if( pOldController ){
		pOldController->FreeReference();
	}
	if( pType ){
		pType->FreeReference();
	}
}



// Management
///////////////

void peeUParameterSetControllerValue::Undo(){
	pParameter->SetControllerValue( pOldController );
}

void peeUParameterSetControllerValue::Redo(){
	pParameter->SetControllerValue( pNewController );
}

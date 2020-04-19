/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeUOCForceFieldSetFluctuationDirection.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/forceField/gdeOCForceField.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCForceFieldSetFluctuationDirection
//////////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCForceFieldSetFluctuationDirection::gdeUOCForceFieldSetFluctuationDirection(
gdeObjectClass *objectClass, gdeOCForceField *forceField, float newValue ) :
pObjectClass( NULL ),
pForceField( NULL )
{
	if( ! objectClass || ! forceField ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Force field set fluctuation direction" );
	
	pOldValue = forceField->GetFluctuationDirection();
	pNewValue = newValue;
	
	pForceField = forceField;
	forceField->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCForceFieldSetFluctuationDirection::~gdeUOCForceFieldSetFluctuationDirection(){
	if( pForceField ){
		pForceField->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCForceFieldSetFluctuationDirection::Undo(){
	pForceField->SetFluctuationDirection( pOldValue );
	pObjectClass->NotifyForceFieldChanged( pForceField );
}

void gdeUOCForceFieldSetFluctuationDirection::Redo(){
	pForceField->SetFluctuationDirection( pNewValue );
	pObjectClass->NotifyForceFieldChanged( pForceField );
}

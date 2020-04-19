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

#include "gdeUOCForceFieldSetTriggerName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCForceFieldSetTriggerName
/////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCForceFieldSetTriggerName::gdeUOCForceFieldSetTriggerName( gdeObjectClass *objectClass,
gdeOCForceField *forceField, gdeOCForceField::eTriggers trigger, const char *newValue ) :
pObjectClass( NULL ),
pForceField( NULL ),
pTrigger( trigger )
{
	if( ! objectClass || ! forceField ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Force field set trigger name" );
	
	pOldValue = forceField->GetTriggerName( trigger );
	pNewValue = newValue;
	
	pForceField = forceField;
	forceField->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCForceFieldSetTriggerName::~gdeUOCForceFieldSetTriggerName(){
	if( pForceField ){
		pForceField->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCForceFieldSetTriggerName::Undo(){
	pForceField->SetTriggerName( pTrigger, pOldValue );
	pObjectClass->NotifyForceFieldChanged( pForceField );
}

void gdeUOCForceFieldSetTriggerName::Redo(){
	pForceField->SetTriggerName( pTrigger, pNewValue );
	pObjectClass->NotifyForceFieldChanged( pForceField );
}

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

#include "gdeUOCLightSetTriggerName.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCLightSetTriggerName
////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCLightSetTriggerName::gdeUOCLightSetTriggerName( gdeObjectClass *objectClass,
gdeOCLight *light, gdeOCLight::eTriggers trigger, const char *newValue ) :
pObjectClass( NULL ),
pLight( NULL ),
pTrigger( trigger )
{
	if( ! objectClass || ! light ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Light set trigger name" );
	
	pOldValue = light->GetTriggerName( trigger );
	pNewValue = newValue;
	
	pLight = light;
	light->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCLightSetTriggerName::~gdeUOCLightSetTriggerName(){
	if( pLight ){
		pLight->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCLightSetTriggerName::Undo(){
	pLight->SetTriggerName( pTrigger, pOldValue );
	pObjectClass->NotifyLightChanged( pLight );
}

void gdeUOCLightSetTriggerName::Redo(){
	pLight->SetTriggerName( pTrigger, pNewValue );
	pObjectClass->NotifyLightChanged( pLight );
}

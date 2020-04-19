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

#include "gdeUOCLightSetRange.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/light/gdeOCLight.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCLightSetRange
//////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCLightSetRange::gdeUOCLightSetRange( gdeObjectClass *objectClass,
gdeOCLight *light, float newValue ) :
pObjectClass( NULL ),
pLight( NULL )
{
	if( ! objectClass || ! light ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Light set range" );
	
	pOldValue = light->GetRange();
	pNewValue = newValue;
	
	pLight = light;
	light->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCLightSetRange::~gdeUOCLightSetRange(){
	if( pLight ){
		pLight->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCLightSetRange::Undo(){
	pLight->SetRange( pOldValue );
	pObjectClass->NotifyLightChanged( pLight );
}

void gdeUOCLightSetRange::Redo(){
	pLight->SetRange( pNewValue );
	pObjectClass->NotifyLightChanged( pLight );
}

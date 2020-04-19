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

#include "gdeUOCRemoveEnvMapProbe.h"
#include "../../../gamedef/gdeGameDefinition.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>



// Class gdeUOCRemoveEnvMapProbe
//////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCRemoveEnvMapProbe::gdeUOCRemoveEnvMapProbe( gdeObjectClass *objectClass, gdeOCEnvMapProbe *envMapProbe ) :
pObjectClass( NULL ),
pEnvMapProbe( NULL )
{
	if( ! objectClass || ! envMapProbe ){
		DETHROW( deeInvalidParam );
	}
	
	if( ! objectClass->GetEnvMapProbes().Has( envMapProbe ) ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Remove environment map probe" );
	
	pEnvMapProbe = envMapProbe;
	envMapProbe->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCRemoveEnvMapProbe::~gdeUOCRemoveEnvMapProbe(){
	if( pEnvMapProbe ){
		pEnvMapProbe->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
}



// Management
///////////////

void gdeUOCRemoveEnvMapProbe::Undo(){
	pObjectClass->GetEnvMapProbes().Add( pEnvMapProbe );
	pObjectClass->NotifyEnvMapProbesChanged();
}

void gdeUOCRemoveEnvMapProbe::Redo(){
	gdeGameDefinition * const gameDefinition = pObjectClass->GetGameDefinition();
	if( gameDefinition && gameDefinition->GetActiveOCEnvMapProbe() ){
		if( gameDefinition->GetSelectedObjectType() == gdeGameDefinition::eotOCEnvMapProbe ){
			gameDefinition->SetSelectedObjectType( gdeGameDefinition::eotObjectClass );
		}
		gameDefinition->SetActiveOCEnvMapProbe( NULL );
	}
	
	pObjectClass->GetEnvMapProbes().Remove( pEnvMapProbe );
	pObjectClass->NotifyEnvMapProbesChanged();
}

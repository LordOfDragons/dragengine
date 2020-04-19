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

#include "gdeUOCEnvMapProbeSetShapeReflection.h"
#include "../../../gamedef/objectClass/gdeObjectClass.h"
#include "../../../gamedef/objectClass/envmapprobe/gdeOCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Class gdeUOCEnvMapProbeSetShapeReflection
//////////////////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeUOCEnvMapProbeSetShapeReflection::gdeUOCEnvMapProbeSetShapeReflection( gdeObjectClass *objectClass,
gdeOCEnvMapProbe *envMapProbe, const decShape *newValue ) :
pObjectClass( NULL ),
pEnvMapProbe( NULL ),
pOldValue( NULL ),
pNewValue( NULL )
{
	if( ! objectClass || ! envMapProbe ){
		DETHROW( deeInvalidParam );
	}
	
	SetShortInfo( "Env-map probe set shape reflection" );
	
	if( envMapProbe->GetShapeReflection() ){
		pOldValue = envMapProbe->GetShapeReflection()->Copy();
	}
	if( newValue ){
		pNewValue = newValue->Copy();
	}
	
	pEnvMapProbe = envMapProbe;
	envMapProbe->AddReference();
	
	pObjectClass = objectClass;
	objectClass->AddReference();
}

gdeUOCEnvMapProbeSetShapeReflection::~gdeUOCEnvMapProbeSetShapeReflection(){
	if( pEnvMapProbe ){
		pEnvMapProbe->FreeReference();
	}
	if( pObjectClass ){
		pObjectClass->FreeReference();
	}
	if( pNewValue ){
		delete pNewValue;
	}
	if( pOldValue ){
		delete pOldValue;
	}
}



// Management
///////////////

void gdeUOCEnvMapProbeSetShapeReflection::Undo(){
	if( pOldValue ){
		pEnvMapProbe->SetShapeReflection( pOldValue->Copy() );
		
	}else{
		pEnvMapProbe->SetShapeReflection( NULL );
	}
	
	pObjectClass->NotifyEnvMapProbeChanged( pEnvMapProbe );
}

void gdeUOCEnvMapProbeSetShapeReflection::Redo(){
	if( pNewValue ){
		pEnvMapProbe->SetShapeReflection( pNewValue->Copy() );
		
	}else{
		pEnvMapProbe->SetShapeReflection( NULL );
	}
	
	pObjectClass->NotifyEnvMapProbeChanged( pEnvMapProbe );
}

/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

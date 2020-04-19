/* 
 * Drag[en]gine IGDE
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
#include <stdlib.h>
#include <string.h>

#include "igdeGDCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Class igdeGDCEnvMapProbe
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCEnvMapProbe::igdeGDCEnvMapProbe(){
	pScaling.Set( 1.0f, 1.0f, 1.0f );
	pShapeReflection = NULL;
	pInfluenceBorderSize = 0.1f;
	pInfluencePriority = 0;
}

igdeGDCEnvMapProbe::igdeGDCEnvMapProbe( const igdeGDCEnvMapProbe &probe ){
	int i;
	
	pShapeReflection = NULL;
	
	try{
		pPosition = probe.GetPosition();
		pOrientation = probe.GetOrientation();
		pScaling = probe.GetScaling();
		
		pShapeListInfluence = probe.GetShapeListInfluence();
		if( probe.GetShapeReflection() ){
			pShapeReflection = probe.GetShapeReflection()->Copy();
		}
		pShapeListReflectionMask = probe.GetShapeListReflectionMask();
		pInfluenceBorderSize = probe.pInfluenceBorderSize;
		pInfluencePriority = probe.pInfluencePriority;
		
		for( i=0; i<=epAttachRotation; i++ ){
			pPropertyNames[ i ] = probe.pPropertyNames[ i ];
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

igdeGDCEnvMapProbe::~igdeGDCEnvMapProbe(){
	pCleanUp();
}



// Management
///////////////

void igdeGDCEnvMapProbe::SetPosition( const decVector &position ){
	pPosition = position;
}

void igdeGDCEnvMapProbe::SetOrientation( const decQuaternion &orientation ){
	pOrientation = orientation;
}

void igdeGDCEnvMapProbe::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}



void igdeGDCEnvMapProbe::SetShapeListInfluence( const decShapeList &shapeList ){
	pShapeListInfluence = shapeList;
}

void igdeGDCEnvMapProbe::SetShapeReflection( decShape *shape ){
	if( shape != pShapeReflection ){
		if( pShapeReflection ){
			delete pShapeReflection;
		}
		
		pShapeReflection = shape;
	}
}

void igdeGDCEnvMapProbe::SetShapeListReflectionMask( const decShapeList &shapeList ){
	pShapeListReflectionMask = shapeList;
}

void igdeGDCEnvMapProbe::SetInfluenceBorderSize( float borderSize ){
	if( borderSize < 0.0f ){
		borderSize = 0.0f;
	}
	
	pInfluenceBorderSize = borderSize;
}

void igdeGDCEnvMapProbe::SetInfluencePriority( int priority ){
	if( priority < 0 ){
		priority = 0;
	}
	
	pInfluencePriority = priority;
}



bool igdeGDCEnvMapProbe::IsPropertySet( int property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString& igdeGDCEnvMapProbe::GetPropertyName( int property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCEnvMapProbe::SetPropertyName( int property, const char *name ){
	if( ! name ){
		DETHROW( deeInvalidParam );
	}
	
	pPropertyNames[ property ] = name;
}

bool igdeGDCEnvMapProbe::HasPropertyWithName( const char *name ) const{
	int j;
	
	for( j=0; j<=epAttachRotation; j++ ){
		if( pPropertyNames[ j ] == name ){
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void igdeGDCEnvMapProbe::pCleanUp(){
	if( pShapeReflection ){
		delete pShapeReflection;
	}
}

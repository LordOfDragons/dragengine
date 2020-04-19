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
#include <stdlib.h>
#include <string.h>

#include "gdeOCEnvMapProbe.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShape.h>



// Class gdeOCEnvMapProbe
///////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCEnvMapProbe::gdeOCEnvMapProbe() :
pScaling( 1.0f, 1.0f, 1.0f ),
pShapeReflection( NULL ),
pInfluenceBorderSize( 0.1f ),
pInfluencePriority( 0 ){
}

gdeOCEnvMapProbe::gdeOCEnvMapProbe( const gdeOCEnvMapProbe &probe ) :
pPosition( probe.GetPosition() ),
pRotation( probe.GetRotation() ),
pScaling( probe.GetScaling() ),
pShapeListInfluence( probe.GetShapeListInfluence() ),
pShapeReflection( NULL ),
pShapeListReflectionMask( probe.GetShapeListReflectionMask() ),
pInfluenceBorderSize( probe.pInfluenceBorderSize ),
pInfluencePriority( probe.pInfluencePriority )
{
	int i;
	
	try{
		if( probe.GetShapeReflection() ){
			pShapeReflection = probe.GetShapeReflection()->Copy();
		}
		
		for( i=0; i<=epAttachRotation; i++ ){
			pPropertyNames[ i ] = probe.pPropertyNames[ i ];
		}
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

gdeOCEnvMapProbe::~gdeOCEnvMapProbe(){
	pCleanUp();
}



// Management
///////////////

void gdeOCEnvMapProbe::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCEnvMapProbe::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCEnvMapProbe::SetScaling( const decVector &scaling ){
	pScaling = scaling;
}



void gdeOCEnvMapProbe::SetShapeReflection( decShape *shape ){
	if( shape == pShapeReflection ){
		return;
	}
	
	if( pShapeReflection ){
		delete pShapeReflection;
	}
	
	pShapeReflection = shape;
}

void gdeOCEnvMapProbe::SetInfluenceBorderSize( float borderSize ){
	pInfluenceBorderSize = decMath::max( borderSize, 0.0f );
}

void gdeOCEnvMapProbe::SetInfluencePriority( int priority ){
	pInfluencePriority = decMath::max( priority, 0 );
}



bool gdeOCEnvMapProbe::IsPropertySet( int property ) const{
	if( property < 0 || property > epAttachRotation ){
		DETHROW( deeInvalidParam );
	}
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString& gdeOCEnvMapProbe::GetPropertyName( int property ) const{
	if( property < 0 || property > epAttachRotation ){
		DETHROW( deeInvalidParam );
	}
	return pPropertyNames[ property ];
}

void gdeOCEnvMapProbe::SetPropertyName( int property, const char *name ){
	if( property < 0 || property > epAttachRotation || ! name ){
		DETHROW( deeInvalidParam );
	}
	pPropertyNames[ property ] = name;
}

bool gdeOCEnvMapProbe::HasPropertyWithName( const char *name ) const{
	int i;
	
	for( i=0; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void gdeOCEnvMapProbe::pCleanUp(){
	if( pShapeReflection ){
		delete pShapeReflection;
	}
}

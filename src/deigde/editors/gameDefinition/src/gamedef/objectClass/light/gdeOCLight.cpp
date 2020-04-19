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

#include "gdeOCLight.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCLight
/////////////////////

// Constructor, destructor
////////////////////////////

gdeOCLight::gdeOCLight() :
pType( deLight::eltPoint ),
pColor( 1.0f, 1.0f, 1.0f ),
pIntensity( 1.0f ),
pRange( 10.0f ),
pHalfIntensityDistance( 0.1f ),
pAmbientRatio( 0.0f ),
pSpotAngle( 30.0f ),
pSpotRatio( 1.0f ),
pSpotSmoothness( 1.0f ),
pSpotExponent( 1.0f ),
pActivated( true ),
pCastShadows( true ),
pHintLightImportance( 100 ),
pHintShadowImportance( 100 ),
pHintMovement( deLight::emhStationary ),
pHintParameter( deLight::ephStatic ){
}

gdeOCLight::gdeOCLight( const gdeOCLight &light ) :
pType( light.pType ),
pColor( light.pColor ),
pIntensity( light.pIntensity ),
pPosition( light.pPosition ),
pRotation( light.pRotation ),
pBoneName( light.pBoneName ),
pRange( light.pRange ),
pHalfIntensityDistance( light.pHalfIntensityDistance ),
pAmbientRatio( light.pAmbientRatio ),
pSpotAngle( light.pSpotAngle ),
pSpotRatio( light.pSpotRatio ),
pSpotSmoothness( light.pSpotSmoothness ),
pSpotExponent( light.pSpotExponent ),
pActivated( light.pActivated ),
pCastShadows( light.pCastShadows ),
pHintLightImportance( light.pHintLightImportance ),
pHintShadowImportance( light.pHintShadowImportance ),
pHintMovement( light.pHintMovement ),
pHintParameter( light.pHintParameter ),
pLightSkinPath( light.pLightSkinPath )
{
	int i;
	
	for( i=epType; i<=epAttachRotation; i++ ){
		pPropertyNames[ i ] = light.pPropertyNames[ i ];
	}
	
	for( i=etActivated; i<=etActivated; i++ ){
		pTriggerNames[ i ] = light.pTriggerNames[ i ];
	}
}

gdeOCLight::~gdeOCLight(){
}



// Management
///////////////

void gdeOCLight::SetType( deLight::eLightTypes type ){
	if( type < deLight::eltPoint || type > deLight::eltProjector ){
		DETHROW( deeInvalidParam );
	}
	pType = type;
}

void gdeOCLight::SetColor( const decColor &color ){
	pColor = color;
}

void gdeOCLight::SetIntensity( float intensity ){
	pIntensity = decMath::max( intensity, 0.0f );
}

void gdeOCLight::SetRange( float range ){
	pRange = decMath::max( range, 0.01f );
}

void gdeOCLight::SetHalfIntensityDistance( float distance ){
	pHalfIntensityDistance = decMath::clamp( distance, 0.01f, 0.99f );
}

void gdeOCLight::SetAmbientRatio( float ratio ){
	pAmbientRatio = decMath::clamp( ratio, 0.0f, 1.0f );
}

void gdeOCLight::SetPosition( const decVector &position ){
	pPosition = position;
}

void gdeOCLight::SetRotation( const decVector &orientation ){
	pRotation = orientation;
}

void gdeOCLight::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}

void gdeOCLight::SetSpotAngle( float angle ){
	pSpotAngle = decMath::clamp( angle, 0.0f, 179.0f );
}

void gdeOCLight::SetSpotRatio( float ratio ){
	pSpotRatio = decMath::max( ratio, 0.0f );
}

void gdeOCLight::SetSpotSmoothness( float smoothness ){
	pSpotSmoothness = decMath::clamp( smoothness, 0.0f, 1.0f );
}

void gdeOCLight::SetSpotExponent( float exponent ){
	pSpotExponent = decMath::max( exponent, 0.0f );
}

void gdeOCLight::SetActivated( bool activated ){
	pActivated = activated;
}

void gdeOCLight::SetCastShadows( bool castShadows ){
	pCastShadows = castShadows;
}

void gdeOCLight::SetHintLightImportance( int importance ){
	pHintLightImportance = decMath::clamp( importance, 0, 100 );
}

void gdeOCLight::SetHintShadowImportance( int importance ){
	pHintShadowImportance = decMath::clamp( importance, 0, 100 );
}

void gdeOCLight::SetHintMovement( deLight::eMovementHints hint ){
	if( hint < deLight::emhStationary || hint > deLight::emhDynamic ){
		DETHROW( deeInvalidParam );
	}
	pHintMovement = hint;
}

void gdeOCLight::SetHintParameter( deLight::eParameterHints hint ){
	if( hint < deLight::ephStatic || hint > deLight::ephDynamic ){
		DETHROW( deeInvalidParam );
	}
	pHintParameter = hint;
}

void gdeOCLight::SetLightSkinPath( const char *path ){
	pLightSkinPath = path;
}



bool gdeOCLight::IsPropertySet( eProperties property ) const{
	if( property < epType || property > epAttachRotation ){
		DETHROW( deeInvalidParam );
	}
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCLight::GetPropertyName( eProperties property ) const{
	if( property < epType || property > epAttachRotation ){
		DETHROW( deeInvalidParam );
	}
	return pPropertyNames[ property ];
}

void gdeOCLight::SetPropertyName( eProperties property, const char *name ){
	if( property < epType || property > epAttachRotation || ! name ){
		DETHROW( deeInvalidParam );
	}
	pPropertyNames[ property ] = name;
}

bool gdeOCLight::HasPropertyWithName( const char *name ) const{
	int i;
	
	for( i=epType; i<=epAttachRotation; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}



bool gdeOCLight::IsTriggerSet( eTriggers trigger ) const{
	if( trigger < etActivated || trigger > etActivated ){
		DETHROW( deeInvalidParam );
	}
	return ! pTriggerNames[ trigger ].IsEmpty();
}

const decString &gdeOCLight::GetTriggerName( eTriggers trigger ) const{
	if( trigger < etActivated || trigger > etActivated ){
		DETHROW( deeInvalidParam );
	}
	return pTriggerNames[ trigger ];
}

void gdeOCLight::SetTriggerName( eTriggers trigger, const char *name ){
	if( trigger < etActivated || trigger > etActivated || ! name ){
		DETHROW( deeInvalidParam );
	}
	pTriggerNames[ trigger ] = name;
}

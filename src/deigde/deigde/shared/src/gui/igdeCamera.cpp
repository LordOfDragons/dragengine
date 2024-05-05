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

#include "igdeCamera.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/camera/deCameraManager.h>



// Class igdeCamera
/////////////////////

// Constructor, destructor
////////////////////////////

igdeCamera::igdeCamera( deEngine *engine ) :
pEngine( engine ),
pEnableHDRR( true ),
pEnableGI( true ),
pDistance( 0.0f )
{
	DEASSERT_NOTNULL( engine )
	
	pEngCamera.TakeOver( engine->GetCameraManager()->CreateCamera() );
	
	pFov = pEngCamera->GetFov() * RAD2DEG;
	pFovRatio = pEngCamera->GetFovRatio();
	pImageDistance = pEngCamera->GetImageDistance();
	pViewDistance = pEngCamera->GetViewDistance();
	
	pEngCamera->SetEnableGI( pEnableGI );
	pEngCamera->SetEnableHDRR( pEnableHDRR );
	pExposure = pEngCamera->GetExposure();
	pLowestIntensity = pEngCamera->GetLowestIntensity();
	pHighestIntensity = pEngCamera->GetHighestIntensity();
	pAdaptionTime = pEngCamera->GetAdaptionTime();
	
	pWhiteIntensity = pEngCamera->GetWhiteIntensity();
	pBloomIntensity = pEngCamera->GetBloomIntensity();
	pBloomStrength = pEngCamera->GetBloomStrength();
	pBloomBlend = pEngCamera->GetBloomBlend();
	pBloomSize = pEngCamera->GetBloomSize();
	pToneMapCurve = pEngCamera->GetToneMapCurve();
	
	SetName( "Camera" );
}

igdeCamera::~igdeCamera(){
	pCleanUp();
}



// Management
///////////////

void igdeCamera::SetEngineWorld( deWorld *world ){
	if( world == pEngWorld ){
		return;
	}
	
	if( pEngWorld ){
		pEngWorld->RemoveCamera( pEngCamera );
	}
	
	pEngWorld = world;
	
	if( world ){
		pEngWorld->AddCamera( pEngCamera );
	}
	
	WorldChanged();
}

void igdeCamera::SetName( const char *name ){
	pName = name;
}

void igdeCamera::SetPosition( const decDVector &position ){
	if( position.IsEqualTo( pPosition ) ){
		return;
	}
	
	pPosition = position;
	
	pUpdateViewMatrix();
	pUpdateCameraPosition();
	
	GeometryChanged();
}

void igdeCamera::SetOrientation( const decVector &orientation ){
	if( orientation.IsEqualTo( pOrientation ) ){
		return;
	}
	
	pOrientation = orientation;
	
	pUpdateViewMatrix();
	pUpdateCameraPosition();
	
	GeometryChanged();
}	

void igdeCamera::SetFov( float fov ){
	if( fabs( fov - pFov ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pFov = fov;
	
	pEngCamera->SetFov( pFov * DEG2RAD );
	
	GeometryChanged();
}

void igdeCamera::SetFovRatio( float fovRatio ){
	if( fabs( fovRatio - pFovRatio ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pFovRatio = fovRatio;
	
	pEngCamera->SetFovRatio( pFovRatio );
	
	GeometryChanged();
}

void igdeCamera::SetImageDistance( float imageDistance ){
	if( fabs( imageDistance - pImageDistance ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pImageDistance = imageDistance;
	
	pEngCamera->SetImageDistance( imageDistance );
	
	GeometryChanged();
}

void igdeCamera::SetViewDistance( float viewDistance ){
	if( fabs( viewDistance - pViewDistance ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pViewDistance = viewDistance;
	
	pEngCamera->SetViewDistance( viewDistance );
	
	GeometryChanged();
}

void igdeCamera::SetEnableHDRR( bool enable ){
	if( enable == pEnableHDRR ){
		return;
	}
	
	pEnableHDRR = enable;
	pEngCamera->SetEnableHDRR( enable );
	
	AdaptionChanged();
}

void igdeCamera::SetExposure( float exposure ){
	exposure = decMath::max( exposure, 0.0f );
	if( fabs( exposure - pExposure ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pExposure = exposure;
	pEngCamera->SetExposure( exposure );
	
	AdaptionChanged();
}

void igdeCamera::SetLowestIntensity( float lowestIntensity ){
	lowestIntensity = decMath::max( lowestIntensity, 0.0f );
	if( fabs( lowestIntensity - pLowestIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pLowestIntensity = lowestIntensity;
	pEngCamera->SetLowestIntensity( lowestIntensity );
	
	AdaptionChanged();
}

void igdeCamera::SetHighestIntensity( float highestIntensity ){
	highestIntensity = decMath::max( highestIntensity, 0.0f );
	if( fabs( highestIntensity - pHighestIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pHighestIntensity = highestIntensity;
	pEngCamera->SetHighestIntensity( highestIntensity );
	
	AdaptionChanged();
}

void igdeCamera::SetAdaptionTime( float adaptionTime ){
	adaptionTime = decMath::max( adaptionTime, 0.0f );
	if( fabs( adaptionTime - pAdaptionTime ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pAdaptionTime = adaptionTime;
	pEngCamera->SetAdaptionTime( adaptionTime );
	
	AdaptionChanged();
}



void igdeCamera::SetEnableGI( bool enable ){
	if( enable == pEnableGI ){
		return;
	}
	
	pEnableGI = enable;
	pEngCamera->SetEnableGI( enable );
	
	AdaptionChanged();
}



void igdeCamera::SetWhiteIntensity( float intensity ){
	intensity = decMath::max( intensity, 0.1f );
	
	if( fabs( intensity - pWhiteIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pWhiteIntensity = intensity;
	pEngCamera->SetWhiteIntensity( intensity );
	
	AdaptionChanged();
}

void igdeCamera::SetBloomIntensity( float intensity ){
	intensity = decMath::max( intensity, 0.0f );
	
	if( fabs( intensity - pBloomIntensity ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBloomIntensity = intensity;
	pEngCamera->SetBloomIntensity( intensity );
	
	AdaptionChanged();
}

void igdeCamera::SetBloomStrength( float strength ){
	strength = decMath::max( strength, 0.0f );
	
	if( fabs( strength - pBloomStrength ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBloomStrength = strength;
	pEngCamera->SetBloomStrength( strength );
	
	AdaptionChanged();
}

void igdeCamera::SetBloomBlend( float blend ){
	blend = decMath::clamp( blend, 0.0f, 1.0f );
	
	if( fabs( blend - pBloomBlend ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBloomBlend = blend;
	pEngCamera->SetBloomBlend( blend );
	
	AdaptionChanged();
}

void igdeCamera::SetBloomSize( float size ){
	size = decMath::clamp( size, 0.0f, 1.0f );
	
	if( fabs( size - pBloomSize ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pBloomSize = size;
	pEngCamera->SetBloomSize( size );
	
	AdaptionChanged();
}



void igdeCamera::SetToneMapCurve( const decCurveBezier &curve ){
	if( curve == pToneMapCurve ){
		return;
	}
	
	pToneMapCurve = curve;
	pEngCamera->SetToneMapCurve( curve );
	
	AdaptionChanged();
}



void igdeCamera::SetDistance( float distance ){
	distance = decMath::max( distance, 0.01f );
	if( fabs( distance - pDistance ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pDistance = distance;
	
	pUpdateViewMatrix();
	pUpdateCameraPosition();
}

void igdeCamera::Reset(){
	SetPosition( decDVector() );
}

decVector igdeCamera::GetDirectionFor( int width, int height, int x, int y ) const{
	float aspectRatio = ( float )width / ( float )height;
	int halfHeight = height / 2;
	int halfWidth = width / 2;
	decVector direction;
	
	direction.x = tanf( pFov * DEG2RAD * 0.5f ) * ( ( float )( x - halfWidth ) / ( float )halfWidth );
	direction.y = tanf( pFov * DEG2RAD * pFovRatio * 0.5f )
		* ( ( float )( halfHeight - y ) / ( float )halfHeight ) / aspectRatio;
	direction.z = 1.0f;
	
	direction.Normalize();
	return pViewMatrix.TransformNormal( direction ).ToVector();
}

void igdeCamera::SetDefaultParameters( float lowestIntensity, float highestIntensity, float adaptionTime ){
	const deCamera::Ref camera( deCamera::Ref::New( pEngine->GetCameraManager()->CreateCamera() ) );
	
	pFov = camera->GetFov() * RAD2DEG;
	pFovRatio = camera->GetFovRatio();
	pImageDistance = camera->GetImageDistance();
	pViewDistance = camera->GetViewDistance();
	
	pEnableGI = true;
	pEnableHDRR = true;
	pExposure = camera->GetExposure();
	pLowestIntensity = lowestIntensity;
	pHighestIntensity = highestIntensity;
	pAdaptionTime = adaptionTime;
	
	pWhiteIntensity = camera->GetWhiteIntensity();
	pBloomIntensity = camera->GetBloomIntensity();
	pBloomStrength = camera->GetBloomStrength();
	pBloomBlend = camera->GetBloomBlend();
	pBloomSize = camera->GetBloomSize();
	pToneMapCurve = camera->GetToneMapCurve();
	
	pEngCamera->SetFov( pFov * DEG2RAD );
	pEngCamera->SetFovRatio( pFovRatio );
	pEngCamera->SetImageDistance( pImageDistance );
	pEngCamera->SetViewDistance( pViewDistance );
	pEngCamera->SetEnableHDRR( pEnableHDRR );
	pEngCamera->SetExposure( pExposure );
	pEngCamera->SetLowestIntensity( pLowestIntensity );
	pEngCamera->SetHighestIntensity( pHighestIntensity );
	pEngCamera->SetAdaptionTime( pAdaptionTime );
	pEngCamera->SetEnableGI( pEnableGI );
	pEngCamera->SetWhiteIntensity( pWhiteIntensity );
	pEngCamera->SetBloomIntensity( pBloomIntensity );
	pEngCamera->SetBloomStrength( pBloomStrength );
	pEngCamera->SetBloomBlend( pBloomBlend );
	pEngCamera->SetBloomSize( pBloomSize );
	pEngCamera->SetToneMapCurve( pToneMapCurve );
	
	GeometryChanged();
	AdaptionChanged();
}



void igdeCamera::WorldChanged(){
}

void igdeCamera::GeometryChanged(){
}

void igdeCamera::ParameterChanged(){
}

void igdeCamera::AdaptionChanged(){
}



// Private Functions
//////////////////////

void igdeCamera::pCleanUp(){
	SetEngineWorld( NULL );
}

void igdeCamera::pUpdateCameraPosition(){
	pEngCamera->SetPosition( pViewMatrix.GetPosition() );
	pEngCamera->SetOrientation( pViewMatrix.ToQuaternion() );
}

void igdeCamera::pUpdateViewMatrix(){
	pViewMatrix = decDMatrix::CreateTranslation( 0.0, 0.0, -pDistance )
		* decDMatrix::CreateRT( pOrientation * DEG2RAD, pPosition );
}

/* 
 * Drag[en]gine Game Engine
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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deCamera.h"
#include "deCameraManager.h"
#include "../effect/deEffect.h"
#include "../effect/deEffectChain.h"
#include "../world/deWorld.h"
#include "../../common/exceptions.h"
#include "../../systems/deGraphicSystem.h"
#include "../../systems/modules/graphic/deBaseGraphicCamera.h"



// Class deCamera
///////////////////

// Constructor, destructor
////////////////////////////

deCamera::deCamera( deCameraManager *manager ) : deResource( manager ){
	pFov = DEG2RAD * 90.0f;
	pFovRatio = 1.0f;
	pImageDistance = 0.01f;
	pViewDistance = 200.0f;
	pExposure = 1.0f;
	pLowestIntensity = 1.0f;
	pHighestIntensity = 1.0f;
	pAdaptionTime = 0.1f;
	
	pEffects = NULL;
	
	pLayerMask.SetBit( 0 );
	
	pPeerGraphic = NULL;
	
	pParentWorld = NULL;
	pLLWorldNext = NULL;
	pLLWorldPrev = NULL;
	
	try{
		pEffects = new deEffectChain;
		if( ! pEffects ) DETHROW( deeOutOfMemory );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deCamera::~deCamera(){
	pCleanUp();
}



// Management
///////////////

void deCamera::SetPosition( const decDVector &position ){
	if( ! position.IsEqualTo( pPosition ) ){
		pPosition = position;
		if( pPeerGraphic ) pPeerGraphic->PositionChanged();
	}
}

void deCamera::SetOrientation( const decQuaternion &orientation ){
	if( ! orientation.IsEqualTo( pOrientation ) ){
		pOrientation = orientation;
		if( pPeerGraphic ) pPeerGraphic->OrientationChanged();
	}
}

void deCamera::SetFov( float fov ){
	if( fov < 0.0f ){
		fov = 0.0f;
		
	}else if( fov >= PI ){
		fov = PI;
	}
	
	if( fabs( fov - pFov ) > 1e-4 ){
		pFov = fov;
		if( pPeerGraphic ) pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetFovRatio( float ratio ){
	if( ratio < 0.0f ) ratio = 1.0f;
	
	if( fabs( ratio - pFovRatio ) > 1e-4 ){
		pFovRatio = ratio;
		if( pPeerGraphic ) pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetImageDistance( float distance ){
	if( distance < 1e-4f ) distance = 1e-4f;
	
	if( fabs( distance - pImageDistance ) > 1e-4 ){
		pImageDistance = distance;
		if( pPeerGraphic ) pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetViewDistance( float distance ){
	if( distance < 1e-3f ) distance = 1e-3f;
	
	if( fabs( distance - pViewDistance ) > 1e-4 ){
		pViewDistance = distance;
		if( pPeerGraphic ) pPeerGraphic->ParameterChanged();
	}
}

void deCamera::SetExposure( float exposure ){
	if( exposure < 0.0f ) exposure = 0.0f;
	
	if( fabs( exposure - pExposure ) > 1e-4 ){
		pExposure = exposure;
		if( pPeerGraphic ) pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetLowestIntensity( float lowestIntensity ){
	if( lowestIntensity < 0.0f ) lowestIntensity = 0.0f;
	
	if( fabs( lowestIntensity - pLowestIntensity ) > 1e-4 ){
		pLowestIntensity = lowestIntensity;
		
		if( pPeerGraphic ) pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetHighestIntensity( float highestIntensity ){
	if( highestIntensity < 0.0f ) highestIntensity = 0.0f;
	
	if( fabs( highestIntensity - pHighestIntensity ) > 1e-4 ){
		pHighestIntensity = highestIntensity;
		
		if( pPeerGraphic ) pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::SetAdaptionTime( float adaptionTime ){
	if( adaptionTime < 0.0f ) adaptionTime = 0.0f;
	
	if( fabs( adaptionTime - pAdaptionTime ) > 1e-4 ){
		pAdaptionTime = adaptionTime;
		
		if( pPeerGraphic ) pPeerGraphic->AdaptionChanged();
	}
}

void deCamera::NotifyLayerMaskChanged(){
	if( pPeerGraphic ){
		pPeerGraphic->LayerMaskChanged();
	}
}

void deCamera::ResetAdaptedIntensity(){
	if( pPeerGraphic ){
		pPeerGraphic->ResetAdaptedIntensity();
	}
}



// Effects
////////////

int deCamera::GetEffectCount() const{
	return pEffects->GetEffectCount();
}

deEffect *deCamera::GetEffectAt( int index ) const{
	return pEffects->GetEffectAt( index );
}

void deCamera::AddEffect( deEffect *effect ){
	pEffects->AddEffect( effect );
	
	if( pPeerGraphic ) pPeerGraphic->EffectAdded( pEffects->GetEffectCount() - 1, effect );
}

void deCamera::RemoveEffect( deEffect *effect ){
	int index = pEffects->IndexOfEffect( effect );
	if( index == -1 ) DETHROW( deeInvalidParam );
	
	effect->AddReference();
	try{
		pEffects->RemoveEffect( effect );
		
		if( pPeerGraphic ) pPeerGraphic->EffectRemoved( index, effect );
		
	}catch( const deException & ){
		effect->FreeReference();
		throw;
	}
	effect->FreeReference();
}

void deCamera::RemoveAllEffects(){
	pEffects->RemoveAllEffects();
	
	if( pPeerGraphic ) pPeerGraphic->AllEffectsRemoved();
}



// System Peers
/////////////////

void deCamera::SetPeerGraphic( deBaseGraphicCamera *peer ){
	if( peer == pPeerGraphic ){
		return;
	}
	
	if( pPeerGraphic ){
		delete pPeerGraphic;
	}
	
	pPeerGraphic = peer;
}



// Linked List
////////////////

void deCamera::SetParentWorld( deWorld *world ){
	pParentWorld = world;
}

void deCamera::SetLLWorldPrev( deCamera *camera ){
	pLLWorldPrev = camera;
}

void deCamera::SetLLWorldNext( deCamera *camera ){
	pLLWorldNext = camera;
}



// Private Functions
//////////////////////

void deCamera::pCleanUp(){
	if( pPeerGraphic ){
		delete pPeerGraphic;
		pPeerGraphic = NULL;
	}
	
	if( pEffects ) delete pEffects;
}

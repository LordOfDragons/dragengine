/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRSky.h"
#include "deoglRSkyLayer.h"
#include "deoglRSkyInstance.h"
#include "deoglRSkyInstanceLayer.h"
#include "deoglRSkyControllerTarget.h"
#include "deoglSkyLayerTracker.h"
#include "deoglSkyLayerGICascade.h"
#include "../configuration/deoglConfiguration.h"
#include "../gi/deoglGIState.h"
#include "../light/shader/deoglLightShader.h"
#include "../light/shader/deoglLightShaderManager.h"
#include "../pipeline/deoglPipelineConfiguration.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTShader.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTChoices.h"
#include "../shaders/paramblock/deoglSPBlockUBO.h"
#include "../skin/channel/deoglSkinChannel.h"
#include "../skin/deoglRSkin.h"
#include "../skin/deoglSkin.h"
#include "../skin/deoglSkinTexture.h"
#include "../shadow/deoglShadowCaster.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/skin/deSkin.h>
#include <dragengine/resources/skin/deSkinTexture.h>
#include <dragengine/resources/sky/deSkyLayer.h>
#include <dragengine/resources/sky/deSkyLayerBody.h>



// Class deoglRSkyInstanceLayer
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRSkyInstanceLayer::deoglRSkyInstanceLayer( deoglRSkyInstance &instance, int index ) :
pInstance( instance ),
pIndex( index ),

pTrackerEnvMap( NULL ),
pSkyNeedsUpdate( false ),

pParamBlockLight( NULL ),
pParamBlockInstance( NULL )
{
	pShadowCaster = new deoglShadowCaster( instance.GetRenderThread() );
}

deoglRSkyInstanceLayer::~deoglRSkyInstanceLayer(){
	RemoveAllGICascades();
	
	if( pTrackerEnvMap ){
		delete pTrackerEnvMap;
	}
	if( pShadowCaster ){
		delete pShadowCaster;
	}
	if( pParamBlockInstance ){
		pParamBlockInstance->FreeReference();
	}
	if( pParamBlockLight ){
		pParamBlockLight->FreeReference();
	}
}



// Management
///////////////

void deoglRSkyInstanceLayer::Update(){
	if( ! pInstance.GetRSky() ){
		pSkyNeedsUpdate = false;
		return;
	}
	
	pUpdateParameters();
	pCheckTracker();
}

void deoglRSkyInstanceLayer::UpdateWithModifiers(){
	const deoglRSkyLayer &layer = pInstance.GetRSky()->GetLayerAt( pIndex );
	
	if( layer.GetMuliplyBySkyLight() ){
		pIntensity *= pInstance.GetTotalSkyLightIntensity();
	}
	
	if( layer.GetMuliplyBySkyColor() ){
		pColor *= pInstance.GetTotalSkyLightColor();
	}
}



bool deoglRSkyInstanceLayer::GetVisible() const{
	return pTransparency > 1e-12f;
}

bool deoglRSkyInstanceLayer::GetHasLightDirect() const{
	return pLightIntensity > 1e-8f;
}

bool deoglRSkyInstanceLayer::GetHasLightAmbient() const{
	return pAmbientIntensity > 1e-8f;
}



deoglLightPipelines &deoglRSkyInstanceLayer::GetPipelines(){
	if( ! pPipelines ){
		pPipelines.TakeOver( new deoglLightPipelinesSky( *this ) );
		pPipelines->Prepare();
	}
	return pPipelines;
}

deoglSPBlockUBO *deoglRSkyInstanceLayer::GetLightParameterBlock(){
	if( ! pParamBlockLight ){
		pParamBlockLight = GetPipelines().GetWithRef( deoglLightPipelines::etNoShadow, 0 ).GetShader()->CreateSPBLightParam();
	}
	return pParamBlockLight;
}

deoglSPBlockUBO *deoglRSkyInstanceLayer::GetInstanceParameterBlock(){
	if( ! pParamBlockInstance ){
		pParamBlockInstance = GetPipelines().GetWithRef( deoglLightPipelines::etNoShadow, 0 ).GetShader()->CreateSPBInstParam();
	}
	return pParamBlockInstance;
}

void deoglRSkyInstanceLayer::NotifyUpdateStaticComponent( deoglRComponent *component ){
	const int count = pGICascades.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( deoglSkyLayerGICascade* )pGICascades.GetAt( i ) )->NotifyUpdateStaticComponent( component );
	}
}



int deoglRSkyInstanceLayer::GetGICascadeCount() const{
	return pGICascades.GetCount();
}

deoglSkyLayerGICascade *deoglRSkyInstanceLayer::GetGICascade( const deoglGICascade &cascade ) const{
	const int count = pGICascades.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSkyLayerGICascade * const slgc = ( deoglSkyLayerGICascade* )pGICascades.GetAt( i );
		if( &slgc->GetGICascade() == &cascade ){
			return slgc;
		}
	}
	
	return NULL;
}

deoglSkyLayerGICascade *deoglRSkyInstanceLayer::AddGICascade( const deoglGICascade &cascade ){
	deoglSkyLayerGICascade *slgc = GetGICascade( cascade );
	if( slgc ){
		return slgc;
	}
	
	slgc = new deoglSkyLayerGICascade( *this, cascade );
	pGICascades.Add( slgc );
	return slgc;
}

void deoglRSkyInstanceLayer::RemoveGICascade( const deoglGICascade &cascade ){
	const int count = pGICascades.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglSkyLayerGICascade * const slgc = ( deoglSkyLayerGICascade* )pGICascades.GetAt( i );
		if( &slgc->GetGICascade() == &cascade ){
			delete slgc;
			pGICascades.RemoveFrom( i );
			return;
		}
	}
}

void deoglRSkyInstanceLayer::RemoveAllGICascades( const deoglGIState &state ){
	const int count = state.GetCascadeCount();
	int i;
	for( i=0; i<count; i++ ){
		RemoveGICascade( state.GetCascadeAt( i ) );
	}
}

void deoglRSkyInstanceLayer::RemoveAllGICascades(){
	int count = pGICascades.GetCount();
	while( count > 0 ){
		delete ( deoglSkyLayerGICascade* )pGICascades.GetAt( --count );
	}
}



// Private Functions
//////////////////////

void deoglRSkyInstanceLayer::pUpdateParameters(){
	const deoglRSkyLayer &layer = pInstance.GetRSky()->GetLayerAt( pIndex );
	const deoglRSkyControllerTarget ** const targets = layer.GetTargets();
	
	// offset
	pOffset = layer.GetOffset();
	if( targets[ deSkyLayer::etOffsetX ] ){
		pOffset.x = targets[ deSkyLayer::etOffsetX ]->GetValue( pInstance, 0.0f );
	}
	if( targets[ deSkyLayer::etOffsetY ] ){
		pOffset.y = targets[ deSkyLayer::etOffsetY ]->GetValue( pInstance, 0.0f );
	}
	if( targets[ deSkyLayer::etOffsetZ ] ){
		pOffset.z = targets[ deSkyLayer::etOffsetZ ]->GetValue( pInstance, 0.0f );
	}
	
	// orientation
	pOrientation = layer.GetOrientation();
	if( targets[ deSkyLayer::etOrientationX ] ){
		pOrientation.x = PI * 2.0f
			* targets[ deSkyLayer::etOrientationX ]->GetValue( pInstance, 0.0f );
	}
	if( targets[ deSkyLayer::etOrientationY ] ){
		pOrientation.y = PI * 2.0f
			* targets[ deSkyLayer::etOrientationY ]->GetValue( pInstance, 0.0f );
	}
	if( targets[ deSkyLayer::etOrientationZ ] ){
		pOrientation.z = PI * 2.0f
			* targets[ deSkyLayer::etOrientationZ ]->GetValue( pInstance, 0.0f );
	}
	
	// rotation
	bool hasRotation = false;
	pRotation.SetZero();
	if( targets[ deSkyLayer::etRotationX ] ){
		pRotation.x = PI * 2.0f * targets[ deSkyLayer::etRotationX ]->GetValue( pInstance, 0.0f );
		hasRotation = true;
	}
	if( targets[ deSkyLayer::etRotationY ] ){
		pRotation.y = PI * 2.0f * targets[ deSkyLayer::etRotationY ]->GetValue( pInstance, 0.0f );
		hasRotation = true;
	}
	if( targets[ deSkyLayer::etRotationZ ] ){
		pRotation.z = PI * 2.0f * targets[ deSkyLayer::etRotationZ ]->GetValue( pInstance, 0.0f );
		hasRotation = true;
	}
	
	// set matrices
	if( hasRotation ){
		pMatrix = decMatrix::CreateRotation( pRotation )
			* decMatrix::CreateRT( pOrientation, pOffset );
		
	}else{
		pMatrix.SetRT( pOrientation, pOffset );
	}
	
	pInvMatrix = pMatrix.Invert();
	
	// color
	pColor = layer.GetColor();
	if( targets[ deSkyLayer::etColorR ] ){
		pColor.r = decMath::max( 0.0f,
			targets[ deSkyLayer::etColorR ]->GetValue( pInstance, 0.0f ) );
	}
	if( targets[ deSkyLayer::etColorG ] ){
		pColor.g = decMath::max( 0.0f,
			targets[ deSkyLayer::etColorG ]->GetValue( pInstance, 0.0f ) );
	}
	if( targets[ deSkyLayer::etColorB ] ){
		pColor.b = decMath::max( 0.0f,
			targets[ deSkyLayer::etColorB ]->GetValue( pInstance, 0.0f ) );
	}
	
	// intensity
	pIntensity = layer.GetIntensity();
	if( targets[ deSkyLayer::etIntensity ] ){
		pIntensity *= decMath::max( 0.0f,
			targets[ deSkyLayer::etIntensity ]->GetValue( pInstance, 1.0f ) );
	}
	
	// transparency
	pTransparency = layer.GetTransparency();
	if( targets[ deSkyLayer::etTransparency ] ){
		pTransparency = decMath::clamp( pTransparency
			* targets[ deSkyLayer::etTransparency ]->GetValue( pInstance, 1.0f ),
				0.0f, 1.0f );
	}
	
	// light orientation
	pLightOrientation = layer.GetLightOrientation();
	
	// light color
	pLightColor = layer.GetLightColor();
	if( targets[ deSkyLayer::etLightColorR ] ){
		pLightColor.r = decMath::max( 0.0f,
			targets[ deSkyLayer::etLightColorR ]->GetValue( pInstance, 0.0f ) );
	}
	if( targets[ deSkyLayer::etLightColorG ] ){
		pLightColor.g = decMath::max( 0.0f,
			targets[ deSkyLayer::etLightColorG ]->GetValue( pInstance, 0.0f ) );
	}
	if( targets[ deSkyLayer::etLightColorB ] ){
		pLightColor.b = decMath::max( 0.0f,
			targets[ deSkyLayer::etLightColorB ]->GetValue( pInstance, 0.0f ) );
	}
	
	// light intensity
	pLightIntensity = layer.GetLightIntensity();
	if( targets[ deSkyLayer::etLightIntensity ] ){
		pLightIntensity *= decMath::max( 0.0f,
			targets[ deSkyLayer::etLightIntensity ]->GetValue( pInstance, 1.0f ) );
	}
	
	// ambient intensity
	pAmbientIntensity = layer.GetAmbientIntensity();
	if( targets[ deSkyLayer::etAmbientIntensity ] ){
		pAmbientIntensity *= decMath::max( 0.0f,
			targets[ deSkyLayer::etAmbientIntensity ]->GetValue( pInstance, 1.0f ) );
	}
}

void deoglRSkyInstanceLayer::pCheckTracker(){
	pSkyNeedsUpdate = false;
	
	if( ! pTrackerEnvMap ){
		pTrackerEnvMap = new deoglSkyLayerTracker;
		
		pTrackerEnvMap->SetThresholdOrientation(
			deoglSkyLayerTracker::THRESHOLD_ONE_DEGREE_ORIENTATION * 1.0f );
		
		pTrackerEnvMap->SetThresholdIntensity( 0.01f );
			// better percentage of camera intensity range perhaps?
		
		pTrackerEnvMap->SetThresholdTransparency( 0.02f ); // 2% change
		
		pTrackerEnvMap->SetThresholdColor( 0.02f );
			// 2% change equals roughly 5 difference in RGB per color component
		
		pSkyNeedsUpdate = true;
	}
	
	// TODO offset
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateOrientation( pMatrix.ToQuaternion() );
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateColor( pColor );
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateIntensity( pIntensity );
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateTransparency( pTransparency );
	
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateLightOrientation( pLightOrientation );
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateLightColor( pLightColor );
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateLightIntensity( pLightIntensity );
	pSkyNeedsUpdate |= pTrackerEnvMap->UpdateAmbientIntensity( pAmbientIntensity );
}

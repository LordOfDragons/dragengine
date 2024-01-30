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

#include "deoglRCamera.h"
#include "deoglRWorld.h"
#include "deoglWorldOctree.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../effects/render/deoglREffect.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../vr/deoglVR.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCamera
///////////////////////

// Constructor, destructor
////////////////////////////

deoglRCamera::deoglRCamera( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pParentWorld( NULL ),

pTextureToneMapParams( NULL ),
pElapsedToneMapAdaption( 0.0f ),
pForceToneMapAdaption( true ),

pEnableHDRR( true ),
pExposure( 1.0f ),
pLowestIntensity( 0.0f ),
pHighestIntensity( 1.0f ),
pAdaptionTime( 1.0f ),

pEnableGI( false ),

pWhiteIntensity( 1.0f ), // 1.5f
pBloomIntensity( 1.5f ), // 2.0f
pBloomStrength( 1.0f ),
pBloomBlend( 1.0f ),
pBloomSize( 0.25f ),

pPlan( NULL ),

pInitTexture( true ),

pLastAverageLuminance( 0.0f ),
pDirtyLastAverageLuminance( true ),

pVR( nullptr )
{
	try{
		// create render plan
		pPlan = new deoglRenderPlan( renderThread );
		pPlan->SetCamera( this );
		
		// create tone mapping parameters texture
		pTextureToneMapParams = new deoglTexture( renderThread );
		pTextureToneMapParams->SetSize( 1, 1 );
		pTextureToneMapParams->SetFBOFormat( 4, renderThread.GetConfiguration().GetUseHDRR() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	LEAK_CHECK_CREATE( renderThread, Camera );
}

deoglRCamera::~deoglRCamera(){
	LEAK_CHECK_FREE( pRenderThread, Camera );
	pCleanUp();
}



// Management
///////////////

void deoglRCamera::SetParentWorld( deoglRWorld *parentWorld ){
	if( parentWorld == pParentWorld ){
		return;
	}
	
	pPlan->SetWorld( NULL ); // has to come first since SetWorld accesses previous world
	
	if( pParentWorld ){
		pParentWorld->FreeReference();
	}
	
	pParentWorld = parentWorld;
	
	if( parentWorld ){
		parentWorld->AddReference();
	}
	
	pPlan->SetWorld( parentWorld );
}



void deoglRCamera::SetPosition( const decDVector &position ){
	pPosition = position;
}



void deoglRCamera::SetCameraMatrices( const decDMatrix &matrix ){
	pCameraMatrix = matrix;
	pInverseCameraMatrix = pCameraMatrix.QuickInvert();
}



void deoglRCamera::SetToneMapParamsTexture( deoglTexture *texture ){
	pTextureToneMapParams = texture;
	pDirtyLastAverageLuminance = true;
}

void deoglRCamera::SetElapsedToneMapAdaption( float elapsed ){
	pElapsedToneMapAdaption = elapsed;
}

void deoglRCamera::SetForceToneMapAdaption( bool forceAdaption ){
	pForceToneMapAdaption = forceAdaption;
	pDirtyLastAverageLuminance |= forceAdaption;
}

void deoglRCamera::ResetElapsedToneMapAdaption(){
	pElapsedToneMapAdaption = 0.0f;
}



void deoglRCamera::SetEnableHDRR( bool enable ){
	pEnableHDRR = enable;
}

void deoglRCamera::SetExposure( float exposure ){
	pExposure = decMath::max( exposure, 0.0f );
}

void deoglRCamera::SetLowestIntensity( float lowestIntensity ){
	pLowestIntensity = decMath::max( lowestIntensity, 0.0f );
}

void deoglRCamera::SetHighestIntensity( float highestIntensity ){
	pHighestIntensity = decMath::max( highestIntensity, 0.0f );
}

void deoglRCamera::SetAdaptionTime( float adaptionTime ){
	pAdaptionTime = decMath::max( adaptionTime, 0.0f );
}



void deoglRCamera::SetEnableGI( bool enable ){
	pEnableGI = enable;
	pPlan->SetUseGIState( enable );
}



void deoglRCamera::SetWhiteIntensity( float intensity ){
	pWhiteIntensity = decMath::max( intensity, 1.0f );
}

void deoglRCamera::SetBloomIntensity( float intensity ){
	pBloomIntensity = decMath::max( intensity, 0.0f );
}

void deoglRCamera::SetBloomStrength( float strength ){
	pBloomStrength = decMath::max( strength, 0.0f );
}

void deoglRCamera::SetBloomBlend( float blend ){
	pBloomBlend = decMath::clamp( blend, 0.0f, 1.0f );
}

void deoglRCamera::SetBloomSize( float size ){
	pBloomSize = decMath::clamp( size, 0.0f, 1.0f );
}

void deoglRCamera::SetToneMapCurve( const decCurveBezier &curve ){
	pToneMapCurve = curve;
}



void deoglRCamera::EnableVR( bool enable ){
	if( enable ){
		if( ! pVR ){
			pVR = new deoglVR( *this );
		}
		
		pRenderThread.SetVRCamera( this );
		
	}else{
		if( pRenderThread.GetVRCamera() == this ){
			pRenderThread.SetVRCamera( nullptr );
		}
		
		if( pVR ){
			delete pVR;
			pVR = nullptr;
		}
	}
}



float deoglRCamera::GetLastAverageLuminance(){
	if( pDirtyLastAverageLuminance ){
		pDirtyLastAverageLuminance = false;
		
		if( pInitTexture || pForceToneMapAdaption ){
			pLastAverageLuminance = pHighestIntensity * pRenderThread.GetConfiguration().GetHDRRSceneKey();
			
		}else{
			const deoglPixelBuffer::Ref pbToneMapParams( deoglPixelBuffer::Ref::New(
				new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, 1, 1, 1 ) ) );
			pTextureToneMapParams->GetPixels( pbToneMapParams );
			pLastAverageLuminance = pbToneMapParams->GetPointerFloat4()->r;
		}
	}
	
	return pLastAverageLuminance;
}



int deoglRCamera::GetEffectCount() const{
	return pEffects.GetCount();
}

deoglREffect &deoglRCamera::GetEffectAt( int index ) const{
	return *( deoglREffect* )pEffects.GetAt( index );
}

void deoglRCamera::AddEffect( deoglREffect *effect ){
	pEffects.Add( effect );
}

void deoglRCamera::RemoveAllEffects(){
	pEffects.RemoveAll();
}



void deoglRCamera::Update( float elapsed ){
	pElapsedToneMapAdaption += elapsed;
	pPlan->GetDirectEnvMapFader().Update( elapsed );
}

void deoglRCamera::PrepareForRender(){
	if( pInitTexture ){
		const deoglPixelBuffer::Ref pbToneMapParams( deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer( deoglPixelBuffer::epfFloat4, 1, 1, 1 ) ) );
		deoglPixelBuffer::sFloat4 &dataToneMapParams = *pbToneMapParams->GetPointerFloat4();
		dataToneMapParams.r = pRenderThread.GetConfiguration().GetHDRRSceneKey(); // averageLuminance
		dataToneMapParams.g = 0.0f; // scaleLum
		dataToneMapParams.b = 0.0f; // lwhite
		dataToneMapParams.a = 0.0f; // brightPassThreshold
		pTextureToneMapParams->SetPixels( pbToneMapParams );
		pInitTexture = false;
		pForceToneMapAdaption = true;
		pDirtyLastAverageLuminance = true;
	}
	
	const int effectCount = pEffects.GetCount();
	int i;
	for( i=0; i<effectCount; i++ ){
		( ( deoglREffect* )pEffects.GetAt( i ) )->PrepareForRender();
	}
}



// Private Functions
//////////////////////

void deoglRCamera::pCleanUp(){
	EnableVR( false );
	SetParentWorld( NULL );
	
	RemoveAllEffects();
	
	delete pPlan;
	
	if( pTextureToneMapParams ){
		delete pTextureToneMapParams;
	}
}

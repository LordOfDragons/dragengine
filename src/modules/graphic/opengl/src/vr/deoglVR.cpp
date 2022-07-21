/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include <stdlib.h>
#include <string.h>

#include "deoglVR.h"
#include "../deGraphicOpenGl.h"
#include "../capabilities/deoglCapabilities.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../model/deoglModel.h"
#include "../model/deoglRModel.h"
#include "../rendering/deoglRenderWorld.h"
#include "../rendering/plan/deoglRenderPlan.h"
#include "../rendering/defren/deoglDeferredRendering.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTDebug.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTLogger.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTChoices.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../world/deoglRCamera.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>


// Class deoglVR
//////////////////

// Constructors and Destructors
/////////////////////////////////

deoglVR::deoglVR( deoglRCamera &camera ) :
pCamera( camera ),
pLeftEye( *this, deBaseVRModule::evreLeft ),
pRightEye( *this, deBaseVRModule::evreRight ),
pCameraFov( 1.0f ),
pCameraFovRatio( 1.0f ),
pState( esBeginFrame ),
pTimeHistoryFrame( 9, 2 ),
pTargetFPS( 90 ),
pTargetFPSHysteresis( 0.1f ), // 0.2f
pUseRenderStereo( false )
{
	// WARNING called from main thread.
	// 
	// for this reason initialization is delayed until BeginFrame.
}

deoglVR::~deoglVR(){
}



// Management
///////////////

void deoglVR::UpdateTargetFPS( float elapsed ){
	const int forceFPS = pCamera.GetRenderThread().GetConfiguration().GetVRForceFrameRate();
	
	if( forceFPS == 0 ){
		const float avgFrameTime = pTimeHistoryFrame.GetAverage();
		const float avgFrameTimeSafe = avgFrameTime * ( 1.0f + pTargetFPSHysteresis );
		
		const int targetFPS = pCalcTargetFPS( avgFrameTime );
		const int targetFPSSafe = pCalcTargetFPS( avgFrameTimeSafe );
		
		if( targetFPS < pTargetFPS ){
			pCamera.GetRenderThread().GetLogger().LogInfoFormat(
				"VR FrameLimiter: Decrease target FPS from %d to %d", pTargetFPS, targetFPS );
			pTargetFPS = targetFPS;
			
		}else if( targetFPSSafe > pTargetFPS ){
			pCamera.GetRenderThread().GetLogger().LogInfoFormat(
				"VR FrameLimiter: Increase target FPS from %d to %d", pTargetFPS, targetFPSSafe );
			pTargetFPS = targetFPSSafe;
		}
		
	}else if( forceFPS != pTargetFPS ){ 
		pCamera.GetRenderThread().GetLogger().LogInfoFormat(
			"VR FrameLimiter: Force target FPS %d", forceFPS );
		pTargetFPS = forceFPS;
	}
	
	pTimeHistoryFrame.Add( elapsed );
}



void deoglVR::BeginFrame(){
	if( pState != esBeginFrame ){
		return;
	}
	
	// NOTE not done during constructor since constructor is called from main thread
	
	deoglRenderThread &renderThread = pCamera.GetRenderThread();
	deBaseVRModule * const vrmodule = renderThread.GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	if( ! vrmodule ){
		return;
	}
	
	pLeftEye.BeginFrame( *vrmodule );
	pRightEye.BeginFrame( *vrmodule );
	
	pGetParameters(); // has to come after eye begin frame calls
	
	pUseRenderStereo = renderThread.GetChoices().GetVRRenderStereo();
	
	vrmodule->BeginFrame();
	
	pState = esRender;
}

void deoglVR::Render(){
	if( pState != esRender ){
		return;
	}
	
	pState = esSubmit;
	
	if( pCamera.GetPlan().GetWorld() ){
		pLeftEye.Render();
		pRightEye.Render();
	}
}

void deoglVR::Submit(){
	if( pState != esSubmit ){
		return;
	}
	
	pState = esRender;
	
	deBaseVRModule * const vrmodule = pCamera.GetRenderThread().GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	if( ! vrmodule ){
		return;
	}
	
	// NOTE OpenVR does not disable GL_SCISSOR_TEST. this causes the glBlitFramebuffer used
	//      inside OpenVR to use whatever scissor parameters are in effect by the last call
	//      of the application. this causes rendere artifacts in the HMD. disabling
	//      GL_SCISSOR_TEST fixes this problem. this is also save if OpenVR is fixed
	OGL_CHECK( pCamera.GetRenderThread(), glDisable( GL_SCISSOR_TEST ) );
	
	deoglFramebuffer * const oldFbo = pCamera.GetRenderThread().GetFramebuffer().GetActive();
	
	pLeftEye.Submit( *vrmodule );
	pRightEye.Submit( *vrmodule );
	
	pCamera.GetRenderThread().GetFramebuffer().Activate( oldFbo );
}

void deoglVR::EndFrame(){
	if( pState != esRender ){
		return;
	}
	
	pState = esBeginFrame;
	
	deBaseVRModule * const module = pCamera.GetRenderThread().GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	if( module ){
		module->EndFrame();
	}
}



// Private Functions
//////////////////////

void deoglVR::pGetParameters(){
	const float pl[ 4 ] = {
		fabsf( ( float )pLeftEye.GetProjectionLeft() ),
		fabsf( ( float )pLeftEye.GetProjectionRight() ),
		fabsf( ( float )pLeftEye.GetProjectionTop() ),
		fabsf( ( float )pLeftEye.GetProjectionBottom() )
	};
	
	const float pr[ 4 ] = {
		fabsf( ( float )pRightEye.GetProjectionRight() ),
		fabsf( ( float )pRightEye.GetProjectionRight() ),
		fabsf( ( float )pRightEye.GetProjectionTop() ),
		fabsf( ( float )pRightEye.GetProjectionBottom() )
	};
	
	// tangent values are negative left/top but maybe not always so using max and fabs to be safe
	pFovX = atanf( decMath::max( pl[ 0 ], pr[ 0 ] ) ) + atanf( decMath::max( pl[ 1 ], pr[ 1 ] ) );
	pFovY = atanf( decMath::max( pl[ 2 ], pr[ 2 ] ) ) + atanf( decMath::min( pl[ 3 ], pr[ 3 ] ) );
	
	pCameraFov = pFovY;
	pCameraFovRatio = pFovX / pFovY;
}

int deoglVR::pCalcTargetFPS( float frameTime ) const{
	if( frameTime < 1.0f / 90.0f || ! pTimeHistoryFrame.HasMetrics() ){
		return 90; // we can reach 90Hz. do not frame limit
		
	}else if( frameTime < 1.0f / 45.0f ){
		return 45; // we can reach 45Hz
		
	}else if( frameTime < 1.0f / 30.0f ){
		return 30; // we can reach 30Hz
		
	}else{
		return 15; // we can reach 15Hz
	}
}

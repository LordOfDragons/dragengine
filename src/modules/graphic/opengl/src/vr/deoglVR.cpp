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
#include "../canvas/render/deoglRCanvasView.h"
#include "../debug/deoglDebugTraceGroup.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../devmode/deoglDeveloperMode.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
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



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;
static decTimer dtimerTotal;

#define DEBUG_RESET_TIMER dtimer.Reset(); dtimerTotal.Reset();
#define DEBUG_PRINT_TIMER(what) renderThread.GetLogger().LogInfoFormat( "VR %s = %iys",\
	what, ( int )( dtimer.GetElapsedTime() * 1000000.0 ) ); dtimer.Reset();
#define DEBUG_PRINT_TIMER_TOTAL(what) renderThread.GetLogger().LogInfoFormat( "VR %s = %iys",\
		what, ( int )( dtimerTotal.GetElapsedTime() * 1000000.0 ) ); dtimerTotal.Reset();
#else
#define DEBUG_RESET_TIMER
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif


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
pUseRenderStereo( false ),
pFBOStereo( nullptr ),
pDebugPanelSize( 1024, 512 ),
pDebugPanelRenderSize( 0.4f, 0.2f )
{
	// WARNING called from main thread.
	// 
	// for this reason initialization is delayed until BeginFrame.
	
	pDebugPanelColorTransform.SetScaling( 1.0f, 1.0f, 1.0f, 0.9f );
}

deoglVR::~deoglVR(){
	if( pFBOStereo ){
		delete pFBOStereo;
		pFBOStereo = nullptr;
	}
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

void deoglVR::DropFBOStereo(){
	if( pFBOStereo ){
		delete pFBOStereo;
		pFBOStereo = nullptr;
	}
}



const deoglRenderTarget::Ref &deoglVR::GetRenderTargetDebugPanel(){
	if( ! pRenderTargetDebugPanel ){
		pRenderTargetDebugPanel.TakeOver( new deoglRenderTarget(
			pCamera.GetRenderThread(), pDebugPanelSize, 4, 8 ) );
		pRenderTargetDebugPanel->PrepareFramebuffer();
	}
	
	return pRenderTargetDebugPanel;
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
	
	if( ! pCamera.GetPlan().GetWorld() ){
		return;
	}
	
	deoglRenderThread &renderThread = pCamera.GetRenderThread();
	deoglRCanvas * const debugOverlayCanvas = renderThread.GetCanvasDebugOverlay();
	if( debugOverlayCanvas ){
		debugOverlayCanvas->PrepareForRender( nullptr );
		debugOverlayCanvas->PrepareForRenderRender( nullptr );
	}
	
	const deoglDebugTraceGroup debugTrace( renderThread, "VR.Render" );
	if( ! pUseRenderStereo ){
		pLeftEye.Render();
		pRightEye.Render();
		return;
	}
	
	// render using stereo rendering
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	const decPoint &targetSize = pLeftEye.GetTargetSize();
	pRenderStereoSize = ( decVector2( targetSize ) * config.GetVRRenderScale() ).Round();
	
	deoglRenderPlan &plan = pCamera.GetPlan();
	plan.SetViewport( pRenderStereoSize.x, pRenderStereoSize.y );
	plan.SetUpscaleSize( targetSize.x, targetSize .y );
	plan.SetUseUpscaling( pRenderStereoSize != targetSize );
	plan.SetUpsideDown( true );
	plan.SetLodMaxPixelError( config.GetLODMaxPixelError() );
	plan.SetLodLevelOffset( 0 );
	plan.SetRenderStereo( true );
	
	try{
		pRenderStereo();
		
	}catch( const deException & ){
		plan.SetFBOTarget( nullptr );
		plan.SetRenderVR( deoglRenderPlan::ervrNone );
		plan.SetRenderStereo( false );
		throw;
	}
	
	plan.SetFBOTarget( nullptr );
	plan.SetRenderVR( deoglRenderPlan::ervrNone );
	plan.SetRenderStereo( false );
	renderThread.SampleDebugTimerVRRender();
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
	
	const deoglDebugTraceGroup debugTrace( pCamera.GetRenderThread(), "VR.Submit" );
	
	// NOTE OpenVR does not disable GL_SCISSOR_TEST. this causes the glBlitFramebuffer used
	//      inside OpenVR to use whatever scissor parameters are in effect by the last call
	//      of the application. this causes rendere artifacts in the HMD. disabling
	//      GL_SCISSOR_TEST fixes this problem. this is also save if OpenVR is fixed
	//      
	// NOTE since we use pipelines now this has to be done using a pipeline instead
	pCamera.GetRenderThread().GetRenderers().GetWorld().GetPipelineClearBuffers()->Activate();
	
	const deoglRestoreFramebuffer restoreFbo( pCamera.GetRenderThread() ); 
	pLeftEye.Submit( *vrmodule );
	pRightEye.Submit( *vrmodule );
}

void deoglVR::EndFrame(){
	if( pState != esRender ){
		return;
	}
	
	const deoglDebugTraceGroup debugTrace( pCamera.GetRenderThread(), "VR.EndFrame" );
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

void deoglVR::pRenderStereo(){
	deoglRenderThread &renderThread = pCamera.GetRenderThread();
	
	DEBUG_RESET_TIMER
	// prepare render target and fbo
	pLeftEye.GetRenderTarget()->PrepareFramebuffer();
	pRightEye.GetRenderTarget()->PrepareFramebuffer();
	
	if( ! pFBOStereo ){
		pFBOStereo = new deoglFramebuffer( renderThread, false );
		
		renderThread.GetFramebuffer().Activate( pFBOStereo );
		
		pFBOStereo->AttachColorTexture( 0, pLeftEye.GetRenderTarget()->GetTexture() );
		pFBOStereo->AttachColorTexture( 1, pRightEye.GetRenderTarget()->GetTexture() );
		
		const GLenum buffers[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		OGL_CHECK( renderThread, pglDrawBuffers( 2, buffers ) );
		OGL_CHECK( renderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		pFBOStereo->Verify();
	}
	
	// render using render plan
	deoglRenderPlan &plan = pCamera.GetPlan();
	
	plan.SetRenderVR( deoglRenderPlan::ervrStereo );
	
	const decMatrix &matrixViewToLeftEye = pLeftEye.GetMatrixViewToEye();
	const decMatrix &matrixViewToRightEye = pRightEye.GetMatrixViewToEye();
	
	plan.SetCameraMatrix( pCamera.GetCameraMatrix().QuickMultiply( matrixViewToLeftEye ) );
	plan.SetCameraStereoMatrix( matrixViewToLeftEye.QuickInvert().QuickMultiply( matrixViewToRightEye ) );
	
	plan.SetFBOTarget( pFBOStereo );
	
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	plan.SetDebugTiming( devmode.GetEnabled() && devmode.GetShowDebugInfo() );
	DEBUG_PRINT_TIMER( "Prepare" )
	
	plan.PrepareRender( nullptr );
	DEBUG_PRINT_TIMER( "RenderPlan Prepare" )
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.Resize( pRenderStereoSize.x, pRenderStereoSize.y, 2 );
	
	plan.Render();
	renderThread.GetRenderers().GetWorld().RenderFinalizeFBO( plan, true, pLeftEye.GetUseGammaCorrection() );
	DEBUG_PRINT_TIMER( "RenderWorld" )
	// set render target dirty?
}

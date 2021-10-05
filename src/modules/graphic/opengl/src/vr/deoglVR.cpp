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
pCameraFov( 1.0f ),
pCameraFovRatio( 1.0f ),
pState( esRender )
{
	deoglRenderThread &renderThread = camera.GetRenderThread();
	
	pGetParameters( renderThread );
	
	// examples on the internet use RGBA8
	pTargetLeftEye.TakeOver( new deoglRenderTarget( renderThread, pRenderSize.x, pRenderSize.y, 4, 8 ) );
	pTargetRightEye.TakeOver( new deoglRenderTarget( renderThread, pRenderSize.x, pRenderSize.y, 4, 8 ) );
}

deoglVR::~deoglVR(){
}



// Management
///////////////

decDMatrix deoglVR::CreateProjectionDMatrix( const sProjection &projection, float znear, float zfar ) const{
	if( znear <= 0.0f || znear >= zfar ){
		DETHROW( deeInvalidParam );
	}
	
	// infinite projective matrix. works for both the inverse depth case and the fallback
	// non-inverse depth case. for fallback it is slightly better than non-infinite thus
	// the same infinite projection matrix can be used for both cases
	const double idx = 1.0 / ( projection.right - projection.left );
	const double idy = 1.0 / ( projection.bottom - projection.top );
	const double sx = projection.right + projection.left;
	const double sy = projection.bottom + projection.top;
	decDMatrix m;
	
	m.a11 = 2.0 * idx;
	m.a12 = 0.0;
	m.a13 = -sx * idx;
	m.a14 = 0.0;
	
	m.a21 = 0.0;
	m.a22 = 2.0 * idy;
	m.a23 = -sy * idy;
	m.a24 = 0.0;
	
	m.a31 = 0.0;
	m.a32 = 0.0;
	
	if( pCamera.GetRenderThread().GetDeferredRendering().GetUseInverseDepth() ){
		// due to inverse depth changing z-clamping
		m.a33 = 0.0;
		m.a34 = znear;
		
	}else{
		m.a33 = 1.0;
		m.a34 = -2.0 * znear;
	}
	
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 1.0;
	m.a44 = 0.0;
	
	return m;
}

decDMatrix deoglVR::CreateFrustumDMatrix( const sProjection &projection, float znear, float zfar ) const{
	// frustum matrix is always non-infinite otherwise SetFrustum calls fail
	const double idx = 1.0 / ( projection.right - projection.left );
	const double idy = 1.0 / ( projection.bottom - projection.top );
	const double idz = 1.0 / ( double )( zfar - znear );
	const double sx = projection.right + projection.left;
	const double sy = projection.bottom + projection.top;
	decDMatrix m;
	
	m.a11 = 2.0 * idx;
	m.a12 = 0.0;
	m.a13 = -sx * idx;
	m.a14 = 0.0;
	
	m.a21 = 0.0;
	m.a22 = 2.0 * idy;
	m.a23 = -sy * idy;
	m.a24 = 0.0;
	
	m.a31 = 0.0;
	m.a32 = 0.0;
	m.a33 = ( zfar + znear ) * idz;
	m.a34 = -2.0 * zfar * znear * idz;
	
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 1.0;
	m.a44 = 0.0;
	
	return m;
}

void deoglVR::BeginFrame(){
	deBaseVRModule * const module = pCamera.GetRenderThread().GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	if( module ){
		module->BeginFrame();
	}
}

void deoglVR::Render(){
	if( pState != esRender || ! pCamera.GetPlan().GetWorld() ){
		return;
	}
	
	deoglRenderThread &renderThread = pCamera.GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	decPoint size( pRenderSize );
	size.x /= config.GetRenderDownScale();
	size.y /= config.GetRenderDownScale();
	
	deoglRenderPlan &plan = pCamera.GetPlan();
	
	plan.SetViewport( 0, 0, size.x, size.y );
	plan.SetUpscaleSize( pRenderSize.x, pRenderSize.y );
	plan.SetUseUpscaling( size != pRenderSize );
	plan.SetUpsideDown( true );
	
	try{
		pRenderLeftEye( renderThread, size );
		pRenderRightEye( renderThread, size );
		
	}catch( const deException & ){
		plan.SetFBOTarget( nullptr );
		plan.SetRenderVR( deoglRenderPlan::ervrNone );
		throw;
	}
	
	plan.SetFBOTarget( nullptr );
	plan.SetRenderVR( deoglRenderPlan::ervrNone );
	
	pState = esSubmit;
}

void deoglVR::Submit(){
	if( pState != esSubmit ){
		return;
	}
	
	pState = esRender;
	
	deBaseVRModule * const module = pCamera.GetRenderThread().GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	if( ! module ){
		return;
	}
	
	const decVector2 tcFrom( 0.0f, 0.0f );
	const decVector2 tcTo( 1.0f, 1.0f );
	
	// NOTE OpenVR does not disable GL_SCISSOR_TEST. this causes the glBlitFramebuffer used
	//      inside OpenVR to use whatever scissor parameters are in effect by the last call
	//      of the application. this causes rendere artifacts in the HMD. disabling
	//      GL_SCISSOR_TEST fixes this problem. this is also save if OpenVR is fixed
	OGL_CHECK( pCamera.GetRenderThread(), glDisable( GL_SCISSOR_TEST ) );
	
	module->SubmitOpenGLTexture2D( deBaseVRModule::evreLeft,
		( void* )( intptr_t )pTargetLeftEye->GetTexture()->GetTexture(), tcFrom, tcTo, false );
	
	module->SubmitOpenGLTexture2D( deBaseVRModule::evreRight,
		( void* )( intptr_t )pTargetRightEye->GetTexture()->GetTexture(), tcFrom, tcTo, false );
}

void deoglVR::EndFrame(){
	deBaseVRModule * const module = pCamera.GetRenderThread().GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	if( module ){
		module->EndFrame();
	}
}



// Private Functions
//////////////////////

void deoglVR::pGetParameters( deoglRenderThread &renderThread ){
	deBaseVRModule &module = *renderThread.GetOgl().GetGameEngine()->GetVRSystem()->GetActiveModule();
	pRenderSize = module.GetRenderSize();
	
	float p[ 4 ];
	module.GetProjectionParameters( deBaseVRModule::evreLeft, p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ] );
	const float pl[ 4 ] = { fabsf( p[ 0 ] ), fabsf( p[ 1 ] ), fabsf( p[ 2 ] ), fabsf( p[ 3 ] ) };
	pProjectionLeftEye.left = p[ 0 ];
	pProjectionLeftEye.right = p[ 1 ];
	pProjectionLeftEye.top = p[ 2 ];
	pProjectionLeftEye.bottom = p[ 3 ];
	
	module.GetProjectionParameters( deBaseVRModule::evreRight, p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ] );
	const float pr[ 4 ] = { fabsf( p[ 0 ] ), fabsf( p[ 1 ] ), fabsf( p[ 2 ] ), fabsf( p[ 3 ] ) };
	pProjectionRightEye.left = p[ 0 ];
	pProjectionRightEye.right = p[ 1 ];
	pProjectionRightEye.top = p[ 2 ];
	pProjectionRightEye.bottom = p[ 3 ];
	
	pMatrixViewToLeftEye = module.GetMatrixViewEye( deBaseVRModule::evreLeft );
	pMatrixViewToRightEye = module.GetMatrixViewEye( deBaseVRModule::evreRight );
	
	pMatrixRightToLeftEye = pMatrixViewToRightEye.QuickInvert().QuickMultiply( pMatrixViewToLeftEye );
	
	pCanvasTCFromLeftEye.Set( 0.0f, 0.0f );
	pCanvasTCToLeftEye.Set( 1.0f, 1.0f );
	pCanvasTCFromRightEye.Set( 0.0f, 0.0f );
	pCanvasTCToRightEye.Set( 1.0f, 1.0f );
	
	// tangent values are negative left/top but maybe not always so using max and fabs to be safe
	pFovX = atanf( decMath::max( pl[ 0 ], pr[ 0 ] ) ) + atanf( decMath::max( pl[ 1 ], pr[ 1 ] ) );
	pFovY = atanf( decMath::max( pl[ 2 ], pr[ 2 ] ) ) + atanf( decMath::min( pl[ 3 ], pr[ 3 ] ) );
	
	pCameraFov = pFovY;
	pCameraFovRatio = pFovX / pFovY;
	
	pHiddenMeshLeft = module.GetHiddenArea( deBaseVRModule::evreLeft );
	pHiddenRMeshLeft = pHiddenMeshLeft ? ( ( deoglModel* )pHiddenMeshLeft->GetPeerGraphic() )->GetRModel() : nullptr;
	
	pHiddenMeshRight = module.GetHiddenArea( deBaseVRModule::evreRight );
	pHiddenRMeshRight = pHiddenMeshRight ? ( ( deoglModel* )pHiddenMeshRight->GetPeerGraphic() )->GetRModel() : nullptr;
	
	renderThread.GetLogger().LogInfoFormat( "VR: size=(%d,%d) fov=(%.1f,%.1f)",
		pRenderSize.x, pRenderSize.y, pFovX * RAD2DEG, pFovY * RAD2DEG );
	
	renderThread.GetLogger().LogInfoFormat( "VR: view left eye\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]",
		pMatrixViewToLeftEye.a11, pMatrixViewToLeftEye.a12, pMatrixViewToLeftEye.a13, pMatrixViewToLeftEye.a14,
		pMatrixViewToLeftEye.a21, pMatrixViewToLeftEye.a22, pMatrixViewToLeftEye.a23, pMatrixViewToLeftEye.a24,
		pMatrixViewToLeftEye.a31, pMatrixViewToLeftEye.a32, pMatrixViewToLeftEye.a33, pMatrixViewToLeftEye.a34);
	
	renderThread.GetLogger().LogInfoFormat( "VR: view right eye\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]",
		pMatrixViewToRightEye.a11, pMatrixViewToRightEye.a12, pMatrixViewToRightEye.a13, pMatrixViewToRightEye.a14,
		pMatrixViewToRightEye.a21, pMatrixViewToRightEye.a22, pMatrixViewToRightEye.a23, pMatrixViewToRightEye.a24,
		pMatrixViewToRightEye.a31, pMatrixViewToRightEye.a32, pMatrixViewToRightEye.a33, pMatrixViewToRightEye.a34);
	
	renderThread.GetLogger().LogInfoFormat( "VR: projection left=(%g,%g,%g,%g)",
		pProjectionLeftEye.left, pProjectionLeftEye.right, pProjectionLeftEye.top, pProjectionLeftEye.bottom);
	
	renderThread.GetLogger().LogInfoFormat( "VR: projection right=(%g,%g,%g,%g)",
		pProjectionRightEye.left, pProjectionRightEye.right, pProjectionRightEye.top, pProjectionRightEye.bottom);
}

void deoglVR::pRenderLeftEye( deoglRenderThread &renderThread, const decPoint &size ){
	// prepare render target and fbo
	pTargetLeftEye->SetSize( size.x, size.y );
	pTargetLeftEye->PrepareFramebuffer();
	
	// render using render plan
	deoglRenderPlan &plan = pCamera.GetPlan();
	
	plan.SetRenderVR( deoglRenderPlan::ervrLeftEye );
	plan.SetCameraMatrix( pCamera.GetCameraMatrix().QuickMultiply( pMatrixViewToLeftEye ) );
	plan.SetFBOTarget( pTargetLeftEye->GetFBO() );
	
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	plan.SetDebugTiming( devmode.GetEnabled() && devmode.GetShowDebugInfo() );
	
	plan.PrepareRender( nullptr );
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.Resize( size.x, size.y );
	plan.Render();
	renderThread.GetRenderers().GetWorld().RenderFinalizeFBO( plan, true );
	// set render target dirty?
}

void deoglVR::pRenderRightEye( deoglRenderThread &renderThread, const decPoint &size ){
	// prepare render target and fbo
	pTargetRightEye->SetSize( size.x, size.y );
	pTargetRightEye->PrepareFramebuffer();
	
	// render using render plan
	deoglRenderPlan &plan = pCamera.GetPlan();
	
	plan.SetRenderVR( deoglRenderPlan::ervrRightEye );
	plan.SetCameraMatrix( pCamera.GetCameraMatrix().QuickMultiply( pMatrixViewToRightEye ) );
	plan.SetCameraCorrectionMatrix( pMatrixRightToLeftEye );
	plan.SetFBOTarget( pTargetRightEye->GetFBO() );
	
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	plan.SetDebugTiming( devmode.GetEnabled() && devmode.GetShowDebugInfo() );
	
	plan.PrepareRender( nullptr );
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.Resize( size.x, size.y );
	plan.Render();
	renderThread.GetRenderers().GetWorld().RenderFinalizeFBO( plan, true );
	// set render target dirty?
}

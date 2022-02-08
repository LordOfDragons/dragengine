/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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
#include "deoglVREye.h"
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


// Class deoglVREye
/////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglVREye::deoglVREye( deoglVR &vr, deBaseVRModule::eEye eye ) :
pVR( vr ),
pEye( eye ),
pProjectionLeft( -1.0f ),
pProjectionRight( 1.0f ),
pProjectionTop( 1.0f ),
pProjectionBottom( -1.0f ),
pVRViewImages( nullptr ),
pVRViewImageCount( 0 ),
pAcquiredVRViewImage( -1 ){
}

deoglVREye::~deoglVREye(){
}



// Management
///////////////

decDMatrix deoglVREye::CreateProjectionDMatrix( float znear, float zfar ) const{
	if( znear <= 0.0f || znear >= zfar ){
		DETHROW( deeInvalidParam );
	}
	
	// infinite projective matrix. works for both the inverse depth case and the fallback
	// non-inverse depth case. for fallback it is slightly better than non-infinite thus
	// the same infinite projection matrix can be used for both cases
	const double idx = 1.0 / ( pProjectionRight - pProjectionLeft );
	const double idy = 1.0 / ( pProjectionBottom - pProjectionTop );
	const double sx = pProjectionRight + pProjectionLeft;
	const double sy = pProjectionBottom + pProjectionTop;
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
	
	if( pVR.GetCamera().GetRenderThread().GetDeferredRendering().GetUseInverseDepth() ){
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

decDMatrix deoglVREye::CreateFrustumDMatrix( float znear, float zfar ) const{
	// frustum matrix is always non-infinite otherwise SetFrustum calls fail
	const double idx = 1.0 / ( pProjectionRight - pProjectionLeft );
	const double idy = 1.0 / ( pProjectionBottom - pProjectionTop );
	const double idz = 1.0 / ( double )( zfar - znear );
	const double sx = pProjectionRight + pProjectionLeft;
	const double sy = pProjectionBottom + pProjectionTop;
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

void deoglVREye::BeginFrame( deBaseVRModule &vrmodule ){
	// parameters are queried every frame update. this is required since some VR runtimes
	// can change these parameters on the fly or can provide them at all times. resources
	// are recreated if parameters changed
	pGetParameters( vrmodule );
	
	if( ! pRenderTarget || pTargetSize != pRenderTarget->GetSize() ){
		deoglRenderThread &renderThread = pVR.GetCamera().GetRenderThread();
		pLogParameters( renderThread );
		
		// examples on the internet use RGBA8
		if( pRenderTarget ){
			pRenderTarget->SetSize( pTargetSize );
			
		}else{
			pRenderTarget.TakeOver( new deoglRenderTarget( renderThread, pTargetSize, 4, 8 ) );
		}
	}
}

void deoglVREye::Render(){
	deoglRenderThread &renderThread = pVR.GetCamera().GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	pRenderSize = ( decVector2( pTargetSize ) * config.GetVRRenderScale() ).Round();
	
	// OpenVR uses blitting which is very slow with scaling. use up scaling instead
	deoglRenderPlan &plan = pVR.GetCamera().GetPlan();
	plan.SetViewport( pRenderSize.x, pRenderSize.y );
	plan.SetUpscaleSize( pTargetSize.x, pTargetSize.y );
	plan.SetUseUpscaling( pRenderSize != pTargetSize );
	plan.SetUpsideDown( true );
	
	try{
		pRender( renderThread );
		
	}catch( const deException & ){
		plan.SetFBOTarget( nullptr );
		plan.SetRenderVR( deoglRenderPlan::ervrNone );
		throw;
	}
	
	plan.SetFBOTarget( nullptr );
	plan.SetRenderVR( deoglRenderPlan::ervrNone );
}

void deoglVREye::Submit( deBaseVRModule &vrmodule ){
	// OpenVR uses blitting which is very slow with scaling
	const decVector2 tcFrom( 0.0f, 0.0f );
	const decVector2 tcTo( 1.0f, 1.0f );
// 	const decVector2 tcTo( ( float )pRenderSize.x / ( float )pTargetSize.x,
// 		( float )pRenderSize.y / ( float )pTargetSize.y );
// 	pVR.GetCamera().GetRenderThread().GetLogger().LogInfoFormat("tcTo (%g,%g)", tcTo.x, tcTo.y );
	
	vrmodule.SubmitOpenGLTexture2D( pEye,
		( void* )( intptr_t )pRenderTarget->GetTexture()->GetTexture(), tcFrom, tcTo, false );
}



// Private Functions
//////////////////////

void deoglVREye::pGetParameters( deBaseVRModule &vrmodule ){
	pTargetSize = vrmodule.GetRenderSize();
	
	float p[ 4 ];
	vrmodule.GetProjectionParameters( pEye, p[ 0 ], p[ 1 ], p[ 2 ], p[ 3 ] );
	pProjectionLeft = ( double )p[ 0 ];
	pProjectionRight = ( double )p[ 1 ];
	pProjectionTop = ( double )p[ 2 ];
	pProjectionBottom = ( double )p[ 3 ];
	
	pMatrixViewToEye = vrmodule.GetMatrixViewEye( pEye );
	
	pCanvasTCFrom.Set( 0.0f, 0.0f );
	pCanvasTCTo.Set( 1.0f, 1.0f );
	pCanvasTCFrom.Set( 0.0f, 0.0f );
	pCanvasTCTo.Set( 1.0f, 1.0f );
	
	deModel * const hiddenMesh = vrmodule.GetHiddenArea( pEye );
	if( hiddenMesh != pHiddenMesh ){
		pHiddenMesh = hiddenMesh;
		pHiddenRMesh = nullptr;
		if( pHiddenMesh ){
			pHiddenRMesh = ( ( deoglModel* )pHiddenMesh->GetPeerGraphic() )->GetRModel();
		}
	}
}

void deoglVREye::pLogParameters( deoglRenderThread &renderThread ){
	const char *prefix = "";
	
	switch( pEye ){
	case deBaseVRModule::evreLeft:
		prefix = "VR Left Eye";
		break;
		
	case deBaseVRModule::evreRight:
		prefix = "VR Right Eye";
		break;
	}
	
	renderThread.GetLogger().LogInfoFormat( "%s: size=(%d,%d)", prefix, pTargetSize.x, pTargetSize.y );
	
	renderThread.GetLogger().LogInfoFormat(
		"%s: matrix view to eye:\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]", prefix,
		pMatrixViewToEye.a11, pMatrixViewToEye.a12, pMatrixViewToEye.a13, pMatrixViewToEye.a14,
		pMatrixViewToEye.a21, pMatrixViewToEye.a22, pMatrixViewToEye.a23, pMatrixViewToEye.a24,
		pMatrixViewToEye.a31, pMatrixViewToEye.a32, pMatrixViewToEye.a33, pMatrixViewToEye.a34);
	
	renderThread.GetLogger().LogInfoFormat( "%s: projection=(%g,%g,%g,%g)", prefix,
		pProjectionLeft, pProjectionRight, pProjectionTop, pProjectionBottom);
}

void deoglVREye::pRender( deoglRenderThread &renderThread ){
	// prepare render target and fbo
	pRenderTarget->PrepareFramebuffer();
	
	// render using render plan
	deoglRenderPlan &plan = pVR.GetCamera().GetPlan();
	
	switch( pEye ){
	case deBaseVRModule::evreLeft:
		plan.SetRenderVR( deoglRenderPlan::ervrLeftEye );
		break;
		
	case deBaseVRModule::evreRight:
		plan.SetRenderVR( deoglRenderPlan::ervrRightEye );
		plan.SetCameraCorrectionMatrix( pMatrixViewToEye.QuickInvert().QuickMultiply(
			pVR.GetLeftEye().GetMatrixViewToEye() ) );
		break;
	}
	
	plan.SetCameraMatrix( pVR.GetCamera().GetCameraMatrix().QuickMultiply( pMatrixViewToEye ) );
	plan.SetFBOTarget( pRenderTarget->GetFBO() );
	
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	plan.SetDebugTiming( devmode.GetEnabled() && devmode.GetShowDebugInfo() );
	
	plan.PrepareRender( nullptr );
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.Resize( pRenderSize.x, pRenderSize.y );
	plan.Render();
	renderThread.GetRenderers().GetWorld().RenderFinalizeFBO( plan, true );
	// set render target dirty?
}

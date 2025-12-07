/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>

#include "deoglVR.h"
#include "deoglVREye.h"
#include "../deGraphicOpenGl.h"
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



// #define DO_TIMING

#ifdef DO_TIMING
#include <dragengine/common/utils/decTimer.h>
static decTimer dtimer;
static decTimer dtimerTotal;

#define DEBUG_RESET_TIMER dtimer.Reset(); dtimerTotal.Reset();
#define DEBUG_PRINT_TIMER(what) \
	renderThread.GetLogger().LogInfoFormat("VREye(%s) %s = %iys",\
		pEye == deBaseVRModule::evreRight ? "R" : "L", \
		what, (int)(dtimer.GetElapsedTime() * 1000000.0));\
	dtimer.Reset();
#define DEBUG_PRINT_TIMER_TOTAL(what) \
	renderThread.GetLogger().LogInfoFormat("VREye(%s) %s = %iys",\
		pEye == deBaseVRModule::evreRight ? "R" : "L", \
		what, (int)(dtimerTotal.GetElapsedTime() * 1000000.0));\
	dtimerTotal.Reset();
#else
#define DEBUG_RESET_TIMER
#define DEBUG_PRINT_TIMER(what)
#define DEBUG_PRINT_TIMER_TOTAL(what)
#endif


// Class deoglVREye
/////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglVREye::deoglVREye(deoglVR &vr, deBaseVRModule::eEye eye) :
pVR(vr),
pEye(eye),
pRenderFormat(deBaseVRModule::evrrfRGB16),
pProjectionLeft(-1.0f),
pProjectionRight(1.0f),
pProjectionTop(1.0f),
pProjectionBottom(-1.0f),
pVRGetViewsBuffer(nullptr),
pVRGetViewsBufferSize(0),
pVRViewImages(nullptr),
pVRViewImageCount(0),
pUseGammaCorrection(false){
}

deoglVREye::~deoglVREye(){
	pDestroyEyeViews();
	
	if(pVRGetViewsBuffer){
		delete [] pVRGetViewsBuffer;
	}
}



// Management
///////////////

decDMatrix deoglVREye::CreateProjectionDMatrix(float znear, float zfar) const{
	if(znear <= 0.0f || znear >= zfar){
		DETHROW(deeInvalidParam);
	}
	
	// infinite projective matrix. works for both the inverse depth case and the fallback
	// non-inverse depth case. for fallback it is slightly better than non-infinite thus
	// the same infinite projection matrix can be used for both cases
	const double idx = 1.0 / (pProjectionRight - pProjectionLeft);
	const double idy = 1.0 / (pProjectionBottom - pProjectionTop);
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
	
	if(pVR.GetCamera().GetRenderThread().GetChoices().GetUseInverseDepth()){
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

decDMatrix deoglVREye::CreateFrustumDMatrix(float znear, float zfar) const{
	// frustum matrix is always non-infinite otherwise SetFrustum calls fail
	const double idx = 1.0 / (pProjectionRight - pProjectionLeft);
	const double idy = 1.0 / (pProjectionBottom - pProjectionTop);
	const double idz = 1.0 / (double)(zfar - znear);
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
	m.a33 = (zfar + znear) * idz;
	m.a34 = -2.0 * zfar * znear * idz;
	
	m.a41 = 0.0;
	m.a42 = 0.0;
	m.a43 = 1.0;
	m.a44 = 0.0;
	
	return m;
}

void deoglVREye::BeginFrame(deBaseVRModule &vrmodule){
	// parameters are queried every frame update. this is required since some VR runtimes
	// can change these parameters on the fly or can provide them at all times. resources
	// are recreated if parameters changed
	pGetParameters(vrmodule);
	
	if(!pRenderTarget || pTargetSize != pRenderTarget->GetSize()){
		deoglRenderThread &renderThread = pVR.GetCamera().GetRenderThread();
		pLogParameters(renderThread);
		
		// examples on the internet use RGBA8
		pVR.DropFBOStereo();
		
		if(pRenderTarget){
			pRenderTarget->SetSize(pTargetSize);
			
		}else{
			pRenderTarget.TakeOver(new deoglRenderTarget(renderThread, pTargetSize, 4, 8));
		}
	}
	
	pUpdateEyeViews(vrmodule);
}

void deoglVREye::Render(){
	deoglRenderThread &renderThread = pVR.GetCamera().GetRenderThread();
	const deoglConfiguration &config = renderThread.GetConfiguration();
	
	pRenderSize = (decVector2(pTargetSize) * config.GetVRRenderScale()).Round();
	
	// OpenVR uses blitting which is very slow with scaling. use up scaling instead
	deoglRenderPlan &plan = pVR.GetCamera().GetPlan();
	plan.SetViewport(pRenderSize.x, pRenderSize.y);
	plan.SetUpscaleSize(pTargetSize.x, pTargetSize.y);
	plan.SetUseUpscaling(pRenderSize != pTargetSize);
	plan.SetUpsideDown(true);
	plan.SetLodMaxPixelError(config.GetLODMaxPixelError());
	plan.SetLodLevelOffset(0);
	plan.SetRenderStereo(false);
	
	try{
		pRender(renderThread);
		
	}catch(const deException &){
		plan.SetFBOTarget(nullptr);
		plan.SetRenderVR(deoglRenderPlan::ervrNone);
		throw;
	}
	
	plan.SetFBOTarget(nullptr);
	plan.SetRenderVR(deoglRenderPlan::ervrNone);
}

void deoglVREye::Submit(deBaseVRModule &vrmodule){
	if(!pRenderTarget->GetFBO() || !pRenderTarget->GetTexture()){
		// shutdown protection
		return;
	}

	if(pVRViewImageCount > 0){
		const int acquiredImageIndex = vrmodule.AcquireEyeViewImage(pEye);
		if(acquiredImageIndex == -1){
			// do not render. perhaps we can honor this earlier but right now it is not
			// known if somebody else than the VR headset requires the rendered image.
			// so for the time being we render but we do not submit
			return;
		}
		
		try{
			// we use blitting to copy the image. if we would be using shaders for
			// this we could avoid the up-sampling during rendering. this way we could
			// use a smaller render target which saves memory and allows other users
			// to operate on a sharp image instead of an upscaled one
			deoglRenderThread &renderThread = pVR.GetCamera().GetRenderThread();
			
			renderThread.GetFramebuffer().Activate(pVRViewImages[acquiredImageIndex].fbo);
			
			OGL_CHECK(renderThread, pglBindFramebuffer(
				GL_READ_FRAMEBUFFER, pRenderTarget->GetFBO()->GetFBO()));
			
			const decPoint src1(0, 0);
			const decPoint &src2 = pTargetSize;
			
			const decPoint dest1(pVRViewTCFrom.Multiply(decVector2(pTargetSize)).Round());
			const decPoint dest2(pVRViewTCTo.Multiply(decVector2(pTargetSize)).Round());
			
			OGL_CHECK(renderThread, pglBlitFramebuffer(src1.x, src1.y, src2.x, src2.y,
				dest1.x, dest1.y, dest2.x, dest2.y, GL_COLOR_BUFFER_BIT, GL_NEAREST));
			
			vrmodule.ReleaseEyeViewImage(pEye);
			
		}catch(const deException &){
			vrmodule.ReleaseEyeViewImage(pEye);
			throw;
		}
		
	}else{
		// OpenVR uses blitting which is very slow with scaling
		const decVector2 tcFrom(0.0f, 0.0f);
		const decVector2 tcTo(1.0f, 1.0f);
	// 	const decVector2 tcTo( ( float )pRenderSize.x / ( float )pTargetSize.x,
	// 		( float )pRenderSize.y / ( float )pTargetSize.y );
	// 	pVR.GetCamera().GetRenderThread().GetLogger().LogInfoFormat("tcTo (%g,%g)", tcTo.x, tcTo.y );
		
		vrmodule.SubmitOpenGLTexture2D(pEye,
			(void*)(intptr_t)pRenderTarget->GetTexture()->GetTexture(), tcFrom, tcTo, false);
	}
}



// Private Functions
//////////////////////

void deoglVREye::pGetParameters(deBaseVRModule &vrmodule){
	pTargetSize = vrmodule.GetRenderSize();
	pRenderFormat = vrmodule.GetRenderFormat();
	
	switch(pRenderFormat){
	case deBaseVRModule::evrrfSRGB8:
	case deBaseVRModule::evrrfSRGBA8:
		pUseGammaCorrection = true;
		break;
		
	default:
		pUseGammaCorrection = false;
	}
	
	float p[4];
	vrmodule.GetProjectionParameters(pEye, p[0], p[1], p[2], p[3]);
	pProjectionLeft = (double)p[0];
	pProjectionRight = (double)p[1];
	pProjectionTop = (double)p[2];
	pProjectionBottom = (double)p[3];
	
	pMatrixViewToEye = vrmodule.GetMatrixViewEye(pEye);
	
	vrmodule.GetEyeViewRenderTexCoords(pEye, pVRViewTCFrom, pVRViewTCTo);
	
	pCanvasTCFrom.Set(0.0f, 0.0f);
	pCanvasTCTo.Set(1.0f, 1.0f);
	
	deModel * const hiddenMesh = vrmodule.GetHiddenArea(pEye);
// 	if( hiddenMesh != pHiddenMesh ){
		pHiddenMesh = hiddenMesh;
		pHiddenRMesh = nullptr;
		if(pHiddenMesh){
			deoglModel * const oglModel = (deoglModel*)pHiddenMesh->GetPeerGraphic();
			pHiddenRMesh = oglModel ? oglModel->GetRModel() : nullptr;
		}
// 	}
}

void deoglVREye::pLogParameters(deoglRenderThread &renderThread){
	const char * const prefix = LogPrefix();
	renderThread.GetLogger().LogInfoFormat("%s: size=(%d,%d) format=%d useGamma=%d",
		prefix, pTargetSize.x, pTargetSize.y, pRenderFormat, pUseGammaCorrection);
	
	renderThread.GetLogger().LogInfoFormat(
		"%s: matrix view to eye:\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]\n[%g,%g,%g,%g]", prefix,
		pMatrixViewToEye.a11, pMatrixViewToEye.a12, pMatrixViewToEye.a13, pMatrixViewToEye.a14,
		pMatrixViewToEye.a21, pMatrixViewToEye.a22, pMatrixViewToEye.a23, pMatrixViewToEye.a24,
		pMatrixViewToEye.a31, pMatrixViewToEye.a32, pMatrixViewToEye.a33, pMatrixViewToEye.a34);
	
	renderThread.GetLogger().LogInfoFormat("%s: projection=(%g,%g,%g,%g)", prefix,
		pProjectionLeft, pProjectionRight, pProjectionTop, pProjectionBottom);
}

void deoglVREye::pUpdateEyeViews(deBaseVRModule &vrmodule){
	const int count = vrmodule.GetEyeViewImages(pEye, 0, nullptr);
	
	if(count > 0){
		if(count > pVRGetViewsBufferSize){
			if(pVRGetViewsBuffer){
				delete [] pVRGetViewsBuffer;
				pVRGetViewsBuffer = nullptr;
				pVRGetViewsBufferSize = 0;
			}
			
			pVRGetViewsBuffer = new GLuint[count];
			pVRGetViewsBufferSize = count;
		}
		
		if(vrmodule.GetEyeViewImages(pEye, count, pVRGetViewsBuffer) != count){
			DETHROW(deeInvalidAction);
		}
	}
	
	bool same = count == pVRViewImageCount;
	if(same){
		int i;
		for(i=0; i<count; i++){
			if(pVRViewImages[i].texture != pVRGetViewsBuffer[i]){
				same = false;
				break;
			}
		}
	}
	
	if(same){
		return;
	}
	
	pDestroyEyeViews();
	
	deoglRenderThread &renderThread = pVR.GetCamera().GetRenderThread();
	
	if(count == 0){
		renderThread.GetLogger().LogInfoFormat("%s: view images 0", LogPrefix());
		return;
	}
	
	const deoglRestoreFramebuffer restoreFbo(renderThread);
	const GLenum buffers[1] = {GL_COLOR_ATTACHMENT0};
	
	pVRViewImages = new cViewImage[count];
	
	for(pVRViewImageCount=0; pVRViewImageCount<count; pVRViewImageCount++){
		cViewImage &vi = pVRViewImages[pVRViewImageCount];
		vi.texture = pVRGetViewsBuffer[pVRViewImageCount];
		
		vi.fbo.TakeOverWith(renderThread, false);
		renderThread.GetFramebuffer().Activate(vi.fbo);
		
		vi.fbo->AttachColorTextureLevel(0, vi.texture, 0);
		
		OGL_CHECK(renderThread, pglDrawBuffers(1, buffers));
		OGL_CHECK(renderThread, glReadBuffer(GL_COLOR_ATTACHMENT0));
		
		vi.fbo->Verify();
	}
	
	renderThread.GetLogger().LogInfoFormat("%s: view images %d", LogPrefix(), pVRViewImageCount);
}

void deoglVREye::pDestroyEyeViews(){
	if(!pVRViewImages){
		return;
	}
	
	delete [] pVRViewImages;
	pVRViewImages = nullptr;
	pVRViewImageCount = 0;
}

void deoglVREye::pRender(deoglRenderThread &renderThread){
	DEBUG_RESET_TIMER
	// prepare render target and fbo
	pRenderTarget->PrepareFramebuffer();
	
	// render using render plan
	deoglRenderPlan &plan = pVR.GetCamera().GetPlan();
	
	switch(pEye){
	case deBaseVRModule::evreLeft:
		plan.SetRenderVR(deoglRenderPlan::ervrLeftEye);
		break;
		
	case deBaseVRModule::evreRight:
		plan.SetRenderVR(deoglRenderPlan::ervrRightEye);
		plan.SetCameraStereoMatrix(pVR.GetLeftEye().pMatrixViewToEye.QuickInvert().QuickMultiply(pMatrixViewToEye));
		break;
	}
	
	plan.SetCameraMatrix(pVR.GetCamera().GetCameraMatrix().QuickMultiply(pMatrixViewToEye));
	plan.SetFBOTarget(pRenderTarget->GetFBO());
	
	const deoglDeveloperMode &devmode = renderThread.GetDebug().GetDeveloperMode();
	plan.SetDebugTiming(devmode.GetEnabled() && devmode.GetShowDebugInfo());
	DEBUG_PRINT_TIMER("Prepare")
	
	plan.PrepareRender(nullptr);
	DEBUG_PRINT_TIMER("RenderPlan Prepare")
	
	deoglDeferredRendering &defren = renderThread.GetDeferredRendering();
	defren.Resize(pRenderSize.x, pRenderSize.y);
	
	
	plan.Render();
	renderThread.GetRenderers().GetWorld().RenderFinalizeFBO(plan, true, pUseGammaCorrection);
	DEBUG_PRINT_TIMER("RenderWorld")
	// set render target dirty?
}

const char *deoglVREye::LogPrefix() const{
	switch(pEye){
	case deBaseVRModule::evreLeft:
		return "VREye Left";
		
	case deBaseVRModule::evreRight:
		return "VREye Right";
		
	default:
		return "VREye";
	}
}

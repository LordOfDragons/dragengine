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

#include "deVROpenXR.h"
#include "deoxrBasics.h"
#include "deoxrInstance.h"
#include "deoxrSession.h"
#include "deoxrSystem.h"
#include "deoxrUtils.h"
#include "action/deoxrActionSet.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/modules/deBaseModule.h>
#include <dragengine/systems/modules/graphic/deBaseGraphicModule.h>

#ifdef OS_UNIX
	#include <dragengine/app/deOSUnix.h>
#elif defined OS_W32
	#include <dragengine/app/deOSWindows.h>
#endif


// class deoxrSession
//////////////////////

struct sSwapchainFormatName{
	int64_t format;
	const char *name;
};

static sSwapchainFormatName vSwapchainFormatNamesOpenGL[]{
	{0x8054, "GL_RGB16"}, // escfGlRgb16
	{0x8056, "GL_RGBA4"},
	{0x8057, "GL_RGB5_A1"},
	{0x8058, "GL_RGBA8"}, // escfGlRgba8
	{0x8059, "GL_RGB10_A2"}, // escfGlRgb10a2
	{0x805b, "GL_RGBA16"}, // escfGlRgba16
	{0x81A5, "GL_DEPTH_COMPONENT16"}, // escfGlDepth16
	{0x81A6, "GL_DEPTH_COMPONENT24"}, // escfGlDepth24
	{0x8229, "GL_R8"},
	{0x822a, "GL_R16"},
	{0x822b, "GL_RG8"},
	{0x822c, "GL_RG16"},
	{0x822d, "GL_R16F"},
	{0x822e, "GL_R32F"},
	{0x822f, "GL_RG16F"},
	{0x8233, "GL_R16I"},
	{0x8234, "GL_R16UI"},
	{0x8239, "GL_RG16I"},
	{0x823a, "GL_RG16UI"},
	{0x881a, "GL_RGBA16F"}, // escfGlRgba16f
	{0x881b, "GL_RGB16F"}, // escfGlRgb16f
	{0x88f0, "GL_DEPTH24_STENCIL8"},
	{0x8c3a, "GL_R11F_G11F_B10F"}, // escfGlR11fG11fB10f
	{0x8c41, "GL_SRGB8"}, // escfGlSrgb8
	{0x8c43, "GL_SRGB8_ALPHA8"}, // escfGlSrgb8Alpha8
	{0x8CAC, "GL_DEPTH_COMPONENT32F"}, // escfGlDepth32F
	{0x8CAD, "GL_DEPTH32F_STENCIL8"}, // escfGlDepth32Stencil8
	{0x8d64, "GL_ETC1_RGB8"},
	{0x8d76, "GL_RGBA16UI"}, // escfGlRgba16ui
	{0x8d77, "GL_RGB16UI"}, // escfGlRgb16ui
	{0x8d88, "GL_RGBA16I"}, // escfGlRgba16i
	{0x8d89, "GL_RGB16I"}, // escfGlRgb16i
	{0x8f94, "GL_R8_SNORM"},
	{0x8f95, "GL_RG8_SNORM"},
	{0x8f96, "GL_RGB8_SNORM"}, // escfGlRgb8Snorm
	{0x8f97, "GL_RGBA8_SNORM"}, // escfGlRgba8Snorm
	{0x8f98, "GL_R16_SNORM"},
	{0x8f99, "GL_RG16_SNORM"},
	{0x8f9a, "GL_RGB16_SNORM"}, // escfGlRgb16Snorm
	{0x8f9b, "GL_RGBA16_SNORM"}, // escfGlRgba16Snorm
	{0x8fbd, "GL_SR8"},
	{0x8fbe, "GL_SRG8"},
	{0x9270, "GL_COMPRESSED_R11_EAC"},
	{0x9271, "GL_COMPRESSED_SIGNED_R11_EAC"},
	{0x9272, "GL_COMPRESSED_RG11_EAC"},
	{0x9273, "GL_COMPRESSED_SIGNED_RG11_EAC"},
	{0x9274, "GL_COMPRESSED_RGB8_ETC2"},
	{0x9275, "GL_COMPRESSED_SRGB8_ETC2"},
	{0x9276, "GL_COMPRESSED_RGB8_PUNCHTHROUGH_ALPHA1_ETC2"},
	{0x9277, "GL_COMPRESSED_SRGB8_PUNCHTHROUGH_ALPHA1_ETC2"},
	{0x9278, "GL_COMPRESSED_RGBA8_ETC2_EAC"},
	{0x9279, "GL_COMPRESSED_SRGB8_ALPHA8_ETC2_EAC"},
	{0x93b0, "GL_COMPRESSED_RGBA_ASTC_4x4"},
	{0x93b1, "GL_COMPRESSED_RGBA_ASTC_5x4"},
	{0x93b2, "GL_COMPRESSED_RGBA_ASTC_5x5"},
	{0x93b3, "GL_COMPRESSED_RGBA_ASTC_6x5"},
	{0x93b4, "GL_COMPRESSED_RGBA_ASTC_6x6"},
	{0x93b5, "GL_COMPRESSED_RGBA_ASTC_8x5"},
	{0x93b6, "GL_COMPRESSED_RGBA_ASTC_8x6"},
	{0x93b7, "GL_COMPRESSED_RGBA_ASTC_8x8"},
	{0x93b8, "GL_COMPRESSED_RGBA_ASTC_10x5"},
	{0x93b9, "GL_COMPRESSED_RGBA_ASTC_10x6"},
	{0x93ba, "GL_COMPRESSED_RGBA_ASTC_10x8"},
	{0x93bb, "GL_COMPRESSED_RGBA_ASTC_10x10"},
	{0x93bc, "GL_COMPRESSED_RGBA_ASTC_12x10"},
	{0x93bd, "GL_COMPRESSED_RGBA_ASTC_12x12"},
	{0x93d0, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_4x4"},
	{0x93d1, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x4"},
	{0x93d2, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_5x5"},
	{0x93d3, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x5"},
	{0x93d4, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_6x6"},
	{0x93d5, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x5"},
	{0x93d6, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x6"},
	{0x93d7, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_8x8"},
	{0x93d8, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x5"},
	{0x93d9, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x6"},
	{0x93da, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x8"},
	{0x93db, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_10x10"},
	{0x93dc, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x10"},
	{0x93dd, "GL_COMPRESSED_SRGB8_ALPHA8_ASTC_12x12"},
	{0, nullptr}
};

deoxrSession::deoxrSession(deoxrSystem &system) :
pSystem(system),
pGraphicApi(egaHeadless),
pSession(0),
pRunning(false),
pPredictedDisplayTime(0),
pPredictedDisplayPeriod(0),
pShouldRender(false),
pFrameRunning(false),
pRequestCenterSpaceOrigin(false),
pSwapchainFormats(nullptr),
pSwapchainFormatCount(0),
pLeftEyePose(deoxrUtils::IdentityPose()),
pRightEyePose(deoxrUtils::IdentityPose()),
pIsGACOpenGL(false),
#ifdef OS_ANDROID
	pGACOpenGLDisplay(nullptr),
	pGACOpenGLConfig(nullptr),
	pGACOpenGLContext(nullptr)
#elif defined OS_UNIX
	pGACOpenGLDisplay(nullptr),
	pGACOpenGLDrawable(0),
	pGACOpenGLContext(nullptr)
#elif defined OS_W32
	pGACOpenGLHDC(NULL),
	pGACOpenGLContext(NULL)
#endif
{
	const deoxrInstance &instance = system.GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	
	// initial values based on a vive hmd. graphic modules should not query the perspective
	// value before BeginFrame() has been called but in case this happens these values are
	// used to not be too far away from the actual values
	// 
	// NOTE OpenXR flips up/down and uses angles not projection matrix values
	pLeftEyeFov.angleLeft = atanf(-1.39863f);
	pLeftEyeFov.angleRight = atanf(1.24906f);
	pLeftEyeFov.angleUp = atanf(1.47526f);
	pLeftEyeFov.angleDown = atanf(-1.46793f);
	
	pRightEyeFov.angleLeft = atanf(-1.24382f);
	pRightEyeFov.angleRight = atanf(1.39166f);
	pRightEyeFov.angleUp = atanf(1.47029f);
	pRightEyeFov.angleDown = atanf(-1.45786f);
	
	try{
		// query graphic api connection parameters
		deBaseGraphicModule::sGraphicApiConnection gacon;
		oxr.GetGameEngine()->GetGraphicSystem()->GetActiveModule()->GetGraphicApiConnection(gacon);
		
		#ifdef OS_BEOS
			#error Unsupported
		#elif defined OS_ANDROID
			pGACOpenGLDisplay = (EGLDisplay)gacon.opengl.display;
			pGACOpenGLConfig = (EGLConfig)gacon.opengl.config;
			pGACOpenGLContext = (EGLContext)gacon.opengl.context;
		#elif defined OS_UNIX
			pGACOpenGLDisplay = (Display*)gacon.opengl.display;
			pGACOpenGLDrawable = (GLXDrawable)gacon.opengl.glxDrawable;
			pGACOpenGLContext = (GLXContext)gacon.opengl.glxContext;
		#elif defined OS_W32
			pGACOpenGLHDC = (HDC)gacon.opengl.hDC;
			pGACOpenGLContext = (HGLRC)gacon.opengl.hGLRC;
		#endif
		
		// create session info struct depending on what the graphic module supports
		XrSessionCreateInfo createInfo{XR_TYPE_SESSION_CREATE_INFO};
		createInfo.systemId = system.GetSystemId();
		
		const void *graphicBinding = nullptr;
		
		// opengl
		#ifdef OS_ANDROID
		XrGraphicsBindingOpenGLESAndroidKHR gbopengl;
		#elif defined OS_UNIX
		XrGraphicsBindingOpenGLXlibKHR gbopengl;
		#elif defined OS_W32
		XrGraphicsBindingOpenGLWin32KHR gbopengl;
		#endif
		
		if(instance.SupportsExtension(deoxrInstance::extKHROpenglEnable)){
			oxr.LogInfo("Create Session: Testing OpenGL Support");
			// openxr specification requires this call to be done although the result is not used
			#ifdef OS_ANDROID
				XrGraphicsRequirementsOpenGLESKHR requirements{
					XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_ES_KHR};
				
				instance.xrGetOpenGLESGraphicsRequirementsKHR(
					instance.GetInstance(), system.GetSystemId(), &requirements);
			#else
				XrGraphicsRequirementsOpenGLKHR requirements{
					XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR};
				
				instance.xrGetOpenGLGraphicsRequirementsKHR(
					instance.GetInstance(), system.GetSystemId(), &requirements);
			#endif
			
			// add opengl connection struct
			#ifdef OS_ANDROID
				if(gacon.opengl.display && gacon.opengl.config && gacon.opengl.context){
					oxr.GetGraphicApiOpenGL().Load();
					
					memset(&gbopengl, 0, sizeof(gbopengl));
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_ES_ANDROID_KHR;
					gbopengl.display = pGACOpenGLDisplay;
					gbopengl.config = pGACOpenGLConfig;
					gbopengl.context = pGACOpenGLContext;
					
					pGraphicApi = egaOpenGL;
					graphicBinding = &gbopengl;
					pIsGACOpenGL = true;
					oxr.LogInfo("Create Session: Using OpenGL on Android");
				}
				
			#elif defined OS_UNIX
				if(gacon.opengl.display && gacon.opengl.display
				&& gacon.opengl.glxFBConfig && gacon.opengl.glxDrawable){
					oxr.GetGraphicApiOpenGL().Load();
					
					memset(&gbopengl, 0, sizeof(gbopengl));
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_XLIB_KHR;
					gbopengl.xDisplay = pGACOpenGLDisplay;
					gbopengl.visualid = gacon.opengl.visualid;
					gbopengl.glxFBConfig = (GLXFBConfig)gacon.opengl.glxFBConfig;
					gbopengl.glxDrawable = pGACOpenGLDrawable;
					gbopengl.glxContext = pGACOpenGLContext;
					
					pGraphicApi = egaOpenGL;
					graphicBinding = &gbopengl;
					pIsGACOpenGL = true;
					oxr.LogInfo("Create Session: Using OpenGL on Xlib");
				}
				
			#elif defined OS_W32
				if(gacon.opengl.hDC && gacon.opengl.hGLRC){
					oxr.GetGraphicApiOpenGL().Load();
					
					memset(&gbopengl, 0, sizeof(gbopengl));
					gbopengl.type = XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR;
					gbopengl.hDC = (HDC)gacon.opengl.hDC;
					gbopengl.hGLRC = (HGLRC)gacon.opengl.hGLRC;
					
					pGraphicApi = egaOpenGL;
					graphicBinding = &gbopengl;
					pIsGACOpenGL = true;
					oxr.LogInfo("Create Session: Using OpenGL on Windows");
				}
			#endif
		}
		
		if(!graphicBinding){
			DETHROW_INFO(deeInvalidAction, "no supported graphic api");
		}
		
		createInfo.next = graphicBinding;
		
		// create session
		OXR_CHECK(instance.xrCreateSession(instance.GetInstance(), &createInfo, &pSession));
		
		// create spaces
		pSpaceStageOrigin = deoxrSpace::Ref::New(*this, XR_REFERENCE_SPACE_TYPE_STAGE);
		pSpaceView = deoxrSpace::Ref::New(*this, XR_REFERENCE_SPACE_TYPE_VIEW);
		pSpaceLocalOrigin = deoxrSpace::Ref::New(*this, XR_REFERENCE_SPACE_TYPE_LOCAL);
		
		pMainSpaceOrigin = pSpaceStageOrigin;
		// pMainSpaceOrigin = pSpaceLocalOrigin;
		
		pCreateSpaces();
		
		// enumerate swapchain formats
		pEnumSwapchainFormats();
		
		// create swap chains
		pSwapchainLeftEye = deoxrSwapchain::Ref::New(
			*this, pSystem.GetLeftEyeViewSize(), deoxrSwapchain::etColor);
		pSwapchainRightEye = deoxrSwapchain::Ref::New(
			*this, pSystem.GetRightEyeViewSize(), deoxrSwapchain::etColor);
		
		if(pSystem.GetInstance().SupportsExtension(deoxrInstance::extKHRCompositionLayerDepth)){
			// pSwapchainDepthLeftEye = deoxrSwapchain::Ref::New(
			// 	*this, pSystem.GetLeftEyeViewSize(), deoxrSwapchain::etDepth);
			// pSwapchainDepthRightEye = deoxrSwapchain::Ref::New(
			// 	*this, pSystem.GetRightEyeViewSize(), deoxrSwapchain::etDepth);
		}
		
		if(pIsGACOpenGL){
			// WARNING SteamVR messes with the current context state causing all future OpenGL
			//         calls to fail. not sure why SteamVR unsets the current context but it
			//         breaks everything. i dont know if the spec would actually requires
			//         runtimes to restore current context if they change it but to work
			//         around such annoying runtimes the current context is restored to the
			//         parameters provided by the graphic module
			RestoreOpenGLCurrent();
		}
		
	}catch(const deException &){
		pCleanUp();
		throw;
	}
}

deoxrSession::~deoxrSession(){
	pCleanUp();
}



// Management
///////////////

void deoxrSession::Begin(){
	if(pRunning){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat("Begin Session");
	
	XrSessionBeginInfo beginInfo;
	memset(&beginInfo, 0, sizeof(beginInfo));
	beginInfo.type = XR_TYPE_SESSION_BEGIN_INFO;
	beginInfo.primaryViewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	
	OXR_CHECK(instance.xrBeginSession(pSession, &beginInfo));
	// pRequestCenterSpaceOrigin = true;
	
	pRunning = true;
}

void deoxrSession::End(){
	if(!pRunning){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat("End Session");
	
	EndFrame();
	pAttachedActionSet = nullptr;
	
	if(!instance.GetOxr().GetPreventDeletion()){
		// prevent deletion of graphic api resources that are typically linked to another
		// thread. this will cause memory leaks but better leak than crash if the runtime
		// is buggy or not very resiliant (like SteamVR for example)
		OXR_CHECK(instance.xrEndSession(pSession));
	}
	
	pRunning = false;
	pPredictedDisplayTime = 0;
	pPredictedDisplayPeriod = 0;
	pShouldRender = false;
}

void deoxrSession::ForceEnd(){
	if(!pRunning){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat("Force End Session");
	ForceEndFrame();
	pAttachedActionSet = nullptr;
	if(!instance.GetOxr().GetPreventDeletion()){
		instance.xrEndSession(pSession);
	}
	pRunning = false;
	pPredictedDisplayTime = 0;
	pPredictedDisplayPeriod = 0;
	pShouldRender = false;
}

void deoxrSession::AttachActionSet(deoxrActionSet *actionSet){
	if(!actionSet){
		DETHROW_INFO(deeNullPointer, "actionSet");
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat("Attach Action Set: %s", actionSet->GetLocalizedName().GetString());
	
	XrSessionActionSetsAttachInfo attachInfo;
	memset(&attachInfo, 0, sizeof(attachInfo));
	attachInfo.type = XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO;
	
	XrActionSet actionSets[1] = {actionSet->GetActionSet()};
	attachInfo.countActionSets = 1;
	attachInfo.actionSets = actionSets;
	
	OXR_CHECK(instance.xrAttachSessionActionSets(pSession, &attachInfo));
	
	pAttachedActionSet = actionSet;
}

void deoxrSession::WaitFrame(){
	if(!pRunning || pFrameRunning){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// synchronization
	XrFrameState state;
	memset(&state, 0, sizeof(state));
	state.type = XR_TYPE_FRAME_STATE;
	
	OXR_CHECK(instance.xrWaitFrame(pSession, nullptr, &state));
	
	pPredictedDisplayTime = state.predictedDisplayTime;
	pPredictedDisplayPeriod = state.predictedDisplayPeriod;
	pShouldRender = state.shouldRender;
	
	if(pRequestCenterSpaceOrigin){
		pRequestCenterSpaceOrigin = false;
		CenterSpaceOrigin(pPredictedDisplayTime);
	}
}

void deoxrSession::BeginFrame(){
	if(!pRunning || pFrameRunning){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	// begin frame
	OXR_CHECK(instance.xrBeginFrame(pSession, nullptr));
	pFrameRunning = true;
	
	// locate views
	XrViewState viewState;
	memset(&viewState, 0, sizeof(viewState));
	viewState.type = XR_TYPE_VIEW_STATE;
	
	XrView views[2];
	memset(&views, 0, sizeof(views));
	views[0].type = XR_TYPE_VIEW;
	views[1].type = XR_TYPE_VIEW;
	
	XrViewLocateInfo viewLocateInfo;
	memset(&viewLocateInfo, 0, sizeof(viewLocateInfo));
	viewLocateInfo.type = XR_TYPE_VIEW_LOCATE_INFO;
	viewLocateInfo.displayTime = pPredictedDisplayTime;
	viewLocateInfo.space = pMainSpace->GetSpace();
	viewLocateInfo.viewConfigurationType = XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO;
	
	uint32_t viewCount;
	OXR_CHECK(instance.xrLocateViews(pSession, &viewLocateInfo, &viewState, 2, &viewCount, views));
	
	pLeftEyePose = views[0].pose;
	pLeftEyeFov = views[0].fov;
	
	pRightEyePose = views[1].pose;
	pRightEyeFov = views[1].fov;
	
	// if hidden mesh are supported but not existing create and update them. this is done
	// only once unless an event is received the images changed. theoretically if the fov
	// changes the mask could change too but this is nigh impossible to ever happen
	if(instance.SupportsExtension(deoxrInstance::extKHRVisibilityMask)){
		if(!pLeftEyeHiddenMesh){
			pLeftEyeHiddenMesh = deoxrHiddenMesh::Ref::New(*this,
				XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 0);
			UpdateLeftEyeHiddenMesh();
		}
		
		if(!pRightEyeHiddenMesh){
			pRightEyeHiddenMesh = deoxrHiddenMesh::Ref::New(*this,
				XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO, 1);
			UpdateRightEyeHiddenMesh();
		}
	}
	
	// locate hmd
	if(pPredictedDisplayTime > 0){
		pSpaceView->LocateSpace(pMainSpace, pPredictedDisplayTime,
			pHeadPosition, pHeadOrientation, pHeadLinearVelocity, pHeadAngularVelocity);
		
		/*
		instance.GetOxr().LogInfoFormat(
			"Locate HMD: pos=(%g,%g,%g) rot=(%g,%g,%g) lv=(%g,%g,%g) av=(%g,%g,%g)",
			pHeadPosition.x, pHeadPosition.y, pHeadPosition.z,
			pHeadOrientation.GetEulerAngles().x * RAD2DEG,
			pHeadOrientation.GetEulerAngles().y * RAD2DEG,
			pHeadOrientation.GetEulerAngles().z * RAD2DEG,
			pHeadLinearVelocity.x, pHeadLinearVelocity.y, pHeadLinearVelocity.z,
			pHeadAngularVelocity.x * RAD2DEG, pHeadAngularVelocity.y * RAD2DEG,
				pHeadAngularVelocity.z * RAD2DEG);
		*/
		
		// calculate eye matrices transforming from camera space to eye space. we have to do
		// this calculation since OpenXR does not provide this information. this happens
		// though once each frame update so not a performance problem
		const decMatrix headMatrix(decMatrix::CreateWorld(pHeadPosition, pHeadOrientation));
		pLeftEyeMatrix = headMatrix.QuickMultiply(deoxrUtils::Convert(pLeftEyePose).QuickInvert());
		pRightEyeMatrix = headMatrix.QuickMultiply(deoxrUtils::Convert(pRightEyePose).QuickInvert());
		
		/*
		instance.GetOxr().LogInfoFormat("- Left Eye: pos=(%g,%g,%g) rot=(%g,%g,%g)",
			pLeftEyeMatrix.GetPosition().x, pLeftEyeMatrix.GetPosition().y, pLeftEyeMatrix.GetPosition().z,
			pLeftEyeMatrix.GetEulerAngles().x * RAD2DEG, pLeftEyeMatrix.GetEulerAngles().y * RAD2DEG,
				pLeftEyeMatrix.GetEulerAngles().z * RAD2DEG);
		
		instance.GetOxr().LogInfoFormat("- Right Eye: pos=(%g,%g,%g) rot=(%g,%g,%g)",
			pRightEyeMatrix.GetPosition().x, pRightEyeMatrix.GetPosition().y, pRightEyeMatrix.GetPosition().z,
			pRightEyeMatrix.GetEulerAngles().x * RAD2DEG, pRightEyeMatrix.GetEulerAngles().y * RAD2DEG,
				pRightEyeMatrix.GetEulerAngles().z * RAD2DEG);
		*/
	}
}

void deoxrSession::EndFrame(){
	if(!pRunning || !pFrameRunning){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	const deoxrPassthrough::Ref &passthrough = instance.GetOxr().GetPassthrough();
	
	// end frame. views have to be submitted even if not rendered to otherwise runtimes
	// like SteamVR can crash entire OpenGL
	XrFrameEndInfo endInfo;
	memset(&endInfo, 0, sizeof(endInfo));
	
	endInfo.type = XR_TYPE_FRAME_END_INFO;
	endInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	if(passthrough){
		endInfo.environmentBlendMode = passthrough->GetEnvBlendMode();
	}
	endInfo.displayTime = pPredictedDisplayTime;
	
	XrCompositionLayerProjectionView views[2];
	memset(&views, 0, sizeof(views));
	
	const void **nextLayer[2] = {&views[0].next, &views[1].next};
	
	views[0].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
	views[0].subImage.swapchain = pSwapchainLeftEye->GetSwapchain();
	views[0].subImage.imageRect.extent.width = pSwapchainLeftEye->GetSize().x;
	views[0].subImage.imageRect.extent.height = pSwapchainLeftEye->GetSize().y;
	views[0].subImage.imageRect.offset.x = 0;
	views[0].subImage.imageRect.offset.y = 0;
	views[0].pose = pLeftEyePose;
	views[0].fov = pLeftEyeFov;
	
	views[1].type = XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW;
	views[1].subImage.swapchain = pSwapchainRightEye->GetSwapchain();
	views[1].subImage.imageRect.extent.width = pSwapchainRightEye->GetSize().x;
	views[1].subImage.imageRect.extent.height = pSwapchainRightEye->GetSize().y;
	views[1].subImage.imageRect.offset.x = 0;
	views[1].subImage.imageRect.offset.y = 0;
	views[1].pose = pRightEyePose;
	views[1].fov = pRightEyeFov;
	
	if(pSwapchainDepthLeftEye && pSwapchainDepthRightEye){
		XrCompositionLayerDepthInfoKHR depth[2];
		memset(&depth, 0, sizeof(depth));
		
		depth[0].type = XR_TYPE_COMPOSITION_LAYER_DEPTH_INFO_KHR;
		depth[0].subImage.swapchain = pSwapchainDepthLeftEye->GetSwapchain();
		depth[0].subImage.imageRect.extent.width = pSwapchainDepthLeftEye->GetSize().x;
		depth[0].subImage.imageRect.extent.height = pSwapchainDepthLeftEye->GetSize().y;
		depth[0].subImage.imageRect.offset.x = 0;
		depth[0].subImage.imageRect.offset.y = 0;
		depth[0].minDepth = 0.0f; // TODO get parameters from graphic module
		depth[0].maxDepth = 1.0f;
		depth[0].nearZ = 0.01f;
		depth[0].farZ = 100.0f;
		*nextLayer[ 0 ] = &depth[ 0 ];
		nextLayer[0] = &depth[0].next;
		
		depth[1].type = XR_TYPE_COMPOSITION_LAYER_DEPTH_INFO_KHR;
		depth[1].subImage.swapchain = pSwapchainDepthRightEye->GetSwapchain();
		depth[1].subImage.imageRect.extent.width = pSwapchainDepthRightEye->GetSize().x;
		depth[1].subImage.imageRect.extent.height = pSwapchainDepthRightEye->GetSize().y;
		depth[1].subImage.imageRect.offset.x = 0;
		depth[1].subImage.imageRect.offset.y = 0;
		depth[1].minDepth = 0.0f; // TODO get parameters from graphic module
		depth[1].maxDepth = 1.0f;
		depth[1].nearZ = 0.01f;
		depth[1].farZ = 100.0f;
		*nextLayer[ 1 ] = &depth[ 1 ];
		nextLayer[1] = &depth[1].next;
	}
	
	const XrCompositionLayerBaseHeader *layers[2];
	int layerCount = 0;
	
	if(passthrough){
		if(passthrough->ShowPassthroughLayerFB()){
			layers[layerCount++] = (const XrCompositionLayerBaseHeader*)&passthrough->GetCompositeLayerFB();
		}
	}
	
	XrCompositionLayerProjection layerProjection{XR_TYPE_COMPOSITION_LAYER_PROJECTION};
	if(passthrough){
		layerProjection.layerFlags |= passthrough->GetCompositeLayerFlags();
	}
	layerProjection.space = pMainSpace->GetSpace();
	layerProjection.viewCount = 2;
	layerProjection.views = views;
	layers[layerCount++] = (const XrCompositionLayerBaseHeader*)&layerProjection;
	
	endInfo.layerCount = layerCount;
	endInfo.layers = layers;
	
	OXR_CHECK(instance.xrEndFrame(pSession, &endInfo));
	pFrameRunning = false;
	
	if(pIsGACOpenGL){
		// WARNING SteamVR messes with the current context state causing all future OpenGL
		//         calls to fail. not sure why SteamVR unsets the current context but it
		//         breaks everything. i dont know if the spec would actually requires
		//         runtimes to restore current context if they change it but to work
		//         around such annoying runtimes the current context is restored to the
		//         parameters provided by the graphic module
		RestoreOpenGLCurrent();
	}
}

void deoxrSession::ForceEndFrame(){
	if(!pRunning || !pFrameRunning){
		return;
	}
	
	XrFrameEndInfo endInfo;
	memset(&endInfo, 0, sizeof(endInfo));
	endInfo.type = XR_TYPE_FRAME_END_INFO;
	endInfo.environmentBlendMode = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
	endInfo.displayTime = pPredictedDisplayTime;
	endInfo.layerCount = 0;
	pSystem.GetInstance().xrEndFrame(pSession, &endInfo);
	pFrameRunning = false;
}

void deoxrSession::SyncActions(){
	if(!pRunning || !pAttachedActionSet){
		return;
	}
	
	const deoxrInstance &instance = pSystem.GetInstance();
	
	XrActionsSyncInfo syncInfo;
	memset(&syncInfo, 0, sizeof(syncInfo));
	
	syncInfo.type = XR_TYPE_ACTIONS_SYNC_INFO;
	
	XrActiveActionSet activeActionSets[1];
	activeActionSets[0].actionSet = pAttachedActionSet->GetActionSet();
	activeActionSets[0].subactionPath = XR_NULL_PATH;
	
	syncInfo.countActiveActionSets = 1;
	syncInfo.activeActionSets = activeActionSets;
	
	OXR_CHECK(instance.xrSyncActions(pSession, &syncInfo));
}

void deoxrSession::UpdateLeftEyeHiddenMesh(){
	pLeftEyeHiddenMesh->SetFov(pLeftEyeFov);
	pLeftEyeHiddenMesh->UpdateModel();
}

void deoxrSession::UpdateRightEyeHiddenMesh(){
	pRightEyeHiddenMesh->SetFov(pRightEyeFov);
	pRightEyeHiddenMesh->UpdateModel();
}

void deoxrSession::SetSpaceOriginPose(const decMatrix &pose){
	const decVector p(pose.GetPosition());
	const decVector r(pose.GetEulerAngles() * RAD2DEG);
	pSystem.GetInstance().GetOxr().LogInfoFormat(
		"Set space origin pose: position=(%f,%f,%f) orientation=(%f,%f,%f)",
		p.x, p.y, p.z, r.x, r.y, r.z);
	
	pSpaceOriginPose = pose;
	pCreateSpaces();
	
	pSystem.GetInstance().GetOxr().GetDevices().ReferenceSpaceChanged();
}

void deoxrSession::CenterSpaceOrigin(XrTime timeOffset){
	decVector position;
	decQuaternion orientation;
	pSpaceView->LocateSpace(pMainSpaceOrigin, timeOffset, position, orientation);
	
	{
	const decVector r(orientation.GetEulerAngles() * RAD2DEG);
	pSystem.GetInstance().GetOxr().LogInfoFormat(
		"Center space origin: position=(%f,%f,%f) orientation=(%f,%f,%f)",
		position.x, position.y, position.z, r.x, r.y, r.z);
	}
	
	position.y = 0.0f;
	orientation.SetFromEulerY(orientation.GetEulerAngles().y);
	
	SetSpaceOriginPose(decMatrix::CreateWorld(position, orientation)/*.QuickInvert()*/);
}

void deoxrSession::RequestCenterSpaceOrigin(){
	pRequestCenterSpaceOrigin = true;
}

void deoxrSession::RestoreOpenGLCurrent(){
	#ifdef OS_ANDROID
		// nothing
	#elif defined OS_UNIX
		pSystem.GetInstance().GetOxr().GetGraphicApiOpenGL().MakeCurrent(
			pGACOpenGLDisplay, pGACOpenGLDrawable, pGACOpenGLContext);
	#elif defined OS_W32
		pSystem.GetInstance().GetOxr().GetGraphicApiOpenGL().MakeCurrent(
			pGACOpenGLHDC, pGACOpenGLContext);
	#endif
}

bool deoxrSession::HasSwapchainFormat(deoxrSession::eSwapchainFormats format) const{
	int i;
	for(i=0; i<pSwapchainFormatCount; i++){
		if(pSwapchainFormats[i] == format){
			return true;
		}
	}
	return false;
}

const char *deoxrSession::GetSwapchainFormatNameOpenGL(int64_t format, const char *notFound) const{
	const sSwapchainFormatName *next = vSwapchainFormatNamesOpenGL;
	while(next->format){
		if(next->format == format){
			return next->name;
		}
		next++;
	}
	return notFound;
}

void deoxrSession::DebugPrintActiveProfilePath() const{
	if(!pSession){
		return;
	}
	
	deoxrInstance &instance = pSystem.GetInstance();
	pDebugPrintActiveProfilePath(instance.GetPathHead(), "Head");
	pDebugPrintActiveProfilePath(instance.GetPathHandRight(), "Right Hand");
	pDebugPrintActiveProfilePath(instance.GetPathHandLeft(), "Left Hand");
	pDebugPrintActiveProfilePath(instance.GetPathGamepad(), "Gamepad");
}



// Private Functions
//////////////////////

void deoxrSession::pCleanUp(){
	if(pRunning){
		const deoxrInstance &instance = pSystem.GetInstance();
		instance.GetOxr().LogInfoFormat("Exit Session due to shuting down");
		
		OXR_CHECK(instance.xrRequestExitSession(pSession));
		pRunning = false;
		pPredictedDisplayTime = 0;
		pPredictedDisplayPeriod = 0;
		pShouldRender = false;
		
		// wait until ready to exit
		/*
		instance.GetOxr().LogInfoFormat("Waiting for exit request to be acknowledged");
		pSystem.GetInstance().GetOxr().WaitUntilReadyExit();
		
		instance.GetOxr().LogInfoFormat("Exit request acknowledged");
		*/
		End();
	}
	
	pAttachedActionSet = nullptr;
	
	pSwapchainDepthLeftEye = nullptr;
	pSwapchainDepthRightEye = nullptr;
	
	pSwapchainLeftEye = nullptr;
	pSwapchainRightEye = nullptr;
	
	pSpaceStage = nullptr;
	
	pSpaceStageOrigin = nullptr;
	pSpaceView = nullptr;
	pSpaceLocal = nullptr;
	pSpaceLocalOrigin = nullptr;
	pMainSpace = nullptr;
	pMainSpaceOrigin = nullptr;
	
	if(pSession){
		pSystem.GetInstance().GetOxr().GetDeviceProfiles().OnSessionEnd();
		pSystem.GetInstance().xrDestroySession(pSession);
		pSession = XR_NULL_HANDLE;
	}
	
	if(pSwapchainFormats){
		delete [] pSwapchainFormats;
		pSwapchainFormats = nullptr;
	}
}

void deoxrSession::pEnumSwapchainFormats(){
	const deoxrInstance &instance = pSystem.GetInstance();
	instance.GetOxr().LogInfoFormat("Enumerate Swapchain Formats:");
	
	uint32_t count;
	OXR_CHECK(instance.xrEnumerateSwapchainFormats(pSession, 0, &count, nullptr));
	if(count == 0){
		return;
	}
	
	pSwapchainFormats = new int64_t[count];
	OXR_CHECK(instance.xrEnumerateSwapchainFormats(pSession, count, &count, pSwapchainFormats));
	pSwapchainFormatCount = count;
	
	uint32_t i;
	for(i=0; i<count; i++){
		const int format = (int)pSwapchainFormats[i];
		instance.GetOxr().LogInfoFormat("- %s (0x%x)", GetSwapchainFormatNameOpenGL(format, "??"), format);
	}
}

void deoxrSession::pDebugPrintActiveProfilePath(const deoxrPath &path, const char *name) const{
	if(!pAttachedActionSet){
		return;
	}
	
	deoxrInstance &instance = pSystem.GetInstance();
	deVROpenXR &oxr = instance.GetOxr();
	XrInteractionProfileState state{XR_TYPE_INTERACTION_PROFILE_STATE};
	
	if(XR_SUCCEEDED(instance.xrGetCurrentInteractionProfile(pSession, path, &state))){
		const deoxrPath resolved(instance, state.interactionProfile);
		if(resolved.IsNotEmpty()){
			oxr.LogInfoFormat("Interaction profile '%s': %s", name, resolved.GetName().GetString());
			return;
		}
	}
	
	oxr.LogInfoFormat("Interaction profile '%s': -", name);
}

void deoxrSession::pCreateSpaces(){
	pSpaceStage = deoxrSpace::Ref::New(*this, XR_REFERENCE_SPACE_TYPE_STAGE, pSpaceOriginPose);
	pSpaceLocal = deoxrSpace::Ref::New(*this, XR_REFERENCE_SPACE_TYPE_LOCAL, pSpaceOriginPose);
	// pSpaceLocal = deoxrSpace::Ref::New(*this, XR_REFERENCE_SPACE_TYPE_LOCAL_FLOOR);
	
	pMainSpace = pMainSpaceOrigin == pSpaceStageOrigin ? pSpaceStage : pSpaceLocal;
}

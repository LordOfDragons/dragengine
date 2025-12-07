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

#ifndef _DEOXRSESSION_H_
#define _DEOXRSESSION_H_

#include "deoxrBasics.h"
#include "deoxrSpace.h"
#include "deoxrSwapchain.h"
#include "deoxrHiddenMesh.h"
#include "deoxrPath.h"
#include "action/deoxrActionSet.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>

class deoxrSystem;
class deoxrActionSet;


/**
 * Oxr session.
 */
class deoxrSession : public deObject{
public:
	/** Reference. */
	typedef deTObjectReference<deoxrSession> Ref;
	
	
	/** Graphic Api. */
	enum eGraphicApi{
		egaHeadless,
		egaOpenGL
	};
	
	/** Swap chain formats. */
	enum eSwapchainFormats{
		escfGlRgb8Snorm = 0x8f96, //<! GL_RGB8_SNORM
		escfGlRgb16 = 0x8054, //<! GL_RGB16
		escfGlRgb16Snorm = 0x8f9a, //<! GL_RGB16_SNORM
		escfGlRgb16f = 0x881b, //<! GL_RGB16F
		escfGlRgb16i = 0x8d89, //<! GL_RGB16I
		escfGlRgb16ui = 0x8d77, //<! GL_RGB16UI
		escfGlR11fG11fB10f = 0x8c3a, //<! GL_R11F_G11F_B10F
		escfGlSrgb8 = 0x8c41, //<! GL_SRGB8
		
		escfGlRgba8 = 0x8058, //<! GL_RGBA8
		escfGlRgba8Snorm = 0x8f97, //<! GL_RGBA8_SNORM
		escfGlRgba16 = 0x805b, //<! GL_RGBA16
		escfGlRgba16Snorm = 0x8f9b, //<! GL_RGBA16_SNORM
		escfGlRgba16f = 0x881a, //<! GL_RGBA16F
		escfGlRgba16i = 0x8d88, //<! GL_RGBA16I
		escfGlRgba16ui = 0x8d76, //<! GL_RGBA16UI
		escfGlRgb10a2 = 0x8059, //<! GL_RGB10_A2
		escfGlSrgb8Alpha8 = 0x8c43, //<! GL_SRGB8_ALPHA8
		
		escfGlDepth16 = 0x81A5, //<! GL_DEPTH_COMPONENT16
		escfGlDepth24 = 0x81A6, //<! GL_DEPTH_COMPONENT24
		escfGlDepth32F = 0x8CAC, //<! GL_DEPTH_COMPONENT32F
		escfGlDepth32Stencil8 = 0x8CAD //<! GL_DEPTH32F_STENCIL8
	};
	
	
	
private:
	deoxrSystem &pSystem;
	
	eGraphicApi pGraphicApi;
	
	XrSession pSession;
	bool pRunning;
	
	XrTime pPredictedDisplayTime;
	XrDuration pPredictedDisplayPeriod;
	bool pShouldRender, pFrameRunning, pRequestCenterSpaceOrigin;
	deoxrActionSet::Ref pAttachedActionSet;
	
	deoxrSpace::Ref pSpaceStage, pSpaceStageOrigin, pSpaceView, pSpaceLocal, pSpaceLocalOrigin,
		pMainSpace, pMainSpaceOrigin;
	
	int64_t *pSwapchainFormats;
	int pSwapchainFormatCount;
	
	deoxrSwapchain::Ref pSwapchainLeftEye;
	deoxrSwapchain::Ref pSwapchainRightEye;
	deoxrSwapchain::Ref pSwapchainDepthLeftEye;
	deoxrSwapchain::Ref pSwapchainDepthRightEye;
	
	XrPosef pLeftEyePose, pRightEyePose;
	XrFovf pLeftEyeFov, pRightEyeFov;
	
	deoxrHiddenMesh::Ref pLeftEyeHiddenMesh;
	deoxrHiddenMesh::Ref pRightEyeHiddenMesh;
	
	decVector pHeadPosition, pHeadLinearVelocity, pHeadAngularVelocity;
	decQuaternion pHeadOrientation;
	decMatrix pLeftEyeMatrix, pRightEyeMatrix, pSpaceOriginPose;
	
	// graphic api connection
	bool pIsGACOpenGL;
#ifdef OS_ANDROID
	EGLDisplay pGACOpenGLDisplay;
	EGLConfig pGACOpenGLConfig;
	EGLContext pGACOpenGLContext;
#elif defined OS_UNIX
	Display *pGACOpenGLDisplay;
	GLXDrawable pGACOpenGLDrawable;
	GLXContext pGACOpenGLContext;
#elif defined OS_W32
	HDC pGACOpenGLHDC;
	HGLRC pGACOpenGLContext;
#endif
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create session. */
	deoxrSession(deoxrSystem &system);
	
protected:
	/** Clean up space. */
	virtual ~deoxrSession();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** System. */
	inline deoxrSystem &GetSystem() const{return pSystem;}
	
	/** Graphic api. */
	inline eGraphicApi GetGraphicApi() const{return pGraphicApi;}
	
	/** Session. */
	inline XrSession GetSession() const{return pSession;}
	
	/** Session is running. */
	inline bool GetRunning() const{return pRunning;}
	
	/** Predicted display time. */
	inline XrTime GetPredictedDisplayTime() const{return pPredictedDisplayTime;}
	
	/** Begin session. */
	void Begin();
	
	/** End session. */
	void End();
	
	/** Force end session ignoring errors. */
	void ForceEnd();
	
	/** Attached action set or nullptr. */
	inline const deoxrActionSet::Ref &GetAttachedActionSet() const{return pAttachedActionSet;}
	
	/** Attach action set. */
	void AttachActionSet(deoxrActionSet *actionSet);
	
	/** Wait frame. */
	void WaitFrame();
	
	/** Begin frame. */
	void BeginFrame();
	
	/** End frame. */
	void EndFrame();
	
	/** Force end frame ignoring errors. */
	void ForceEndFrame();
	
	/** Frame is running. */
	inline bool GetFrameRunning() const{return pFrameRunning;}
	
	/** Runtime request rendering. */
	inline bool GetShouldRender() const{return pShouldRender;}
	
	/** Sync actions. */
	void SyncActions();
	
	/** Spaces. */
	inline const deoxrSpace::Ref &GetMainSpace() const{return pMainSpace;}
	inline const deoxrSpace::Ref &GetSpaceStage() const{return pSpaceStage;}
	inline const deoxrSpace::Ref &GetSpaceView() const{return pSpaceView;}
	inline const deoxrSpace::Ref &GetSpaceLocal() const{return pSpaceLocal;}

	/** Left eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainLeftEye() const{return pSwapchainLeftEye;}
	
	/** Right eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainRightEye() const{return pSwapchainRightEye;}
	
	/** Depth left eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainDepthLeftEye() const{return pSwapchainDepthLeftEye;}
	
	/** Depth right eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainDepthRightEye() const{return pSwapchainDepthRightEye;}
	
	/** Left eye pose. */
	inline const XrPosef &GetLeftEyePose() const{return pLeftEyePose;}
	
	/** Left eye fov. */
	inline const XrFovf &GetLeftEyeFov() const{return pLeftEyeFov;}
	
	/** Right eye pose. */
	inline const XrPosef &GetRightEyePose() const{return pRightEyePose;}
	
	/** Right eye fov. */
	inline const XrFovf &GetRightEyeFov() const{return pRightEyeFov;}
	
	/** Left eye hidden mesh or null. */
	inline deoxrHiddenMesh *GetLeftEyeHiddenMesh() const{return pLeftEyeHiddenMesh;}
	
	/** Right eye hidden mesh or nullptr. */
	inline deoxrHiddenMesh *GetRightEyeHiddenMesh() const{return pRightEyeHiddenMesh;}
	
	/** Update left eye hidden mesh. */
	void UpdateLeftEyeHiddenMesh();
	
	/** Update right eye hidden mesh. */
	void UpdateRightEyeHiddenMesh();
	
	/** Head position in stage coordinate system. */
	inline const decVector &GetHeadPosition() const{return pHeadPosition;}
	
	/** Head orientation in stage coordinate system. */
	inline const decQuaternion &GetHeadOrientation() const{return pHeadOrientation;}
	
	/** Head linear velocity in stage coordinate system. */
	inline const decVector &GetHeadLinearVelocity() const{return pHeadLinearVelocity;}
	
	/** Head angular velocity in stage coordinate system. */
	inline const decVector &GetHeadAngularVelocity() const{return pHeadAngularVelocity;}
	
	/** Left eye matrix relative to head. */
	inline const decMatrix &GetLeftEyeMatrix() const{return pLeftEyeMatrix;}
	
	/** Right eye matrix relative to head. */
	inline const decMatrix &GetRightEyeMatrix() const{return pRightEyeMatrix;}
	
	/** Origin pose relative to space where player is facing forward. */
	inline const decMatrix &GetSpaceOriginPose() const{return pSpaceOriginPose;}
	
	/** Set origin pose relative to space where player is facing forward. */
	void SetSpaceOriginPose(const decMatrix &pose);
	
	/** Set space origin pose from current HMD pose. */
	void CenterSpaceOrigin(XrTime timeOffset);
	
	/** Request center space origin. */
	void RequestCenterSpaceOrigin();
	
	/** Graphic connection is OpenGL. */
	inline bool GetIsGACOpenGL() const{return pIsGACOpenGL;}
	
	/** Restore OpenGL current. */
	void RestoreOpenGLCurrent();
	
	/** Count of swap chain formats. */
	inline int GetSwapchainFormatCount() const{return pSwapchainFormatCount;}
	
	/** Swap chain formats in the the order of runtime preference. */
	inline const int64_t *GetSwapchainFormats() const{return pSwapchainFormats;}
	
	/** Swapchain format is supported. */
	bool HasSwapchainFormat(eSwapchainFormats format) const;
	
	/** Swapchain format name or 'notFound'. */
	const char *GetSwapchainFormatNameOpenGL(int64_t format, const char *notFound = nullptr) const;
	
	/** Debug print some important device profile path. */
	void DebugPrintActiveProfilePath() const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnumSwapchainFormats();
	void pDebugPrintActiveProfilePath(const deoxrPath &path, const char *name) const;
	void pCreateSpaces();
};

#endif


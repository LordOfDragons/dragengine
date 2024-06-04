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
		escfGlRgba16Ext = 0x805b, //<! GL_RGBA16_EXT
		escfGlRgba16f = 0x881a, //<! GL_RGBA16F
		escfGlRgb16f = 0x881b, //<! GL_RGB16F
		escfGlSrgb8Ext = 0x8c41, //<! GL_SRGB8_EXT
		escfGlSrgb8Alpha8Ext = 0x8c43, //<! GL_SRGB8_ALPHA8_EXT
		escfGlRgba8Ext = 0x8058, //<! GL_RGBA8_EXT
		escfGlR11fG11fB10f = 0x8c3a, //<! GL_R11F_G11F_B10F
		escfGlDepth16 = 0x81A5, //<! GL_DEPTH_COMPONENT16
		escfGlDepth24 = 0x81A6, //<! GL_DEPTH_COMPONENT24
		escfGlDepth32F = 0x8CAC //<! GL_DEPTH_COMPONENT32F
	};
	
	
	
private:
	deoxrSystem &pSystem;
	
	eGraphicApi pGraphicApi;
	
	XrSession pSession;
	bool pRunning;
	
	XrTime pPredictedDisplayTime;
	XrDuration pPredictedDisplayPeriod;
	bool pShouldRender;
	bool pFrameRunning;
	deoxrActionSet::Ref pAttachedActionSet;
	
	deoxrSpace::Ref pSpaceStage;
	deoxrSpace::Ref pSpaceView;
	deoxrSpace::Ref pSpaceLocal;
	
	int64_t *pSwapchainFormats;
	int pSwapchainFormatCount;
	
	deoxrSwapchain::Ref pSwapchainLeftEye;
	deoxrSwapchain::Ref pSwapchainRightEye;
	deoxrSwapchain::Ref pSwapchainDepthLeftEye;
	deoxrSwapchain::Ref pSwapchainDepthRightEye;
	
	XrPosef pLeftEyePose;
	XrFovf pLeftEyeFov;
	
	XrPosef pRightEyePose;
	XrFovf pRightEyeFov;
	
	deoxrHiddenMesh::Ref pLeftEyeHiddenMesh;
	deoxrHiddenMesh::Ref pRightEyeHiddenMesh;
	
	decVector pHeadPosition;
	decQuaternion pHeadOrientation;
	decVector pHeadLinearVelocity;
	decVector pHeadAngularVelocity;
	decMatrix pLeftEyeMatrix;
	decMatrix pRightEyeMatrix;
	
	// graphic api connection
	bool pIsGACOpenGL;
#ifdef OS_UNIX
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
	deoxrSession( deoxrSystem &system );
	
protected:
	/** Clean up space. */
	virtual ~deoxrSession();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** System. */
	inline deoxrSystem &GetSystem() const{ return pSystem; }
	
	/** Graphic api. */
	inline eGraphicApi GetGraphicApi() const{ return pGraphicApi; }
	
	/** Session. */
	inline XrSession GetSession() const{ return pSession; }
	
	/** Session is running. */
	inline bool GetRunning() const{ return pRunning; }
	
	/** Predicted display time. */
	inline XrTime GetPredictedDisplayTime() const{ return pPredictedDisplayTime; }
	
	/** Begin session. */
	void Begin();
	
	/** End session. */
	void End();
	
	/** Force end session ignoring errors. */
	void ForceEnd();
	
	/** Attach action set. */
	void AttachActionSet( deoxrActionSet *actionSet );
	
	/** Wait frame. */
	void WaitFrame();
	
	/** Begin frame. */
	void BeginFrame();
	
	/** End frame. */
	void EndFrame();
	
	/** Force end frame ignoring errors. */
	void ForceEndFrame();
	
	/** Frame is running. */
	inline bool GetFrameRunning() const{ return pFrameRunning; }
	
	/** Runtime request rendering. */
	inline bool GetShouldRender() const{ return pShouldRender; }
	
	/** Sync actions. */
	void SyncActions();
	
	/** Spaces. */
	inline const deoxrSpace::Ref &GetSpace() const{ return pSpaceStage; }
	inline const deoxrSpace::Ref &GetSpaceStage() const{ return pSpaceStage; }
	inline const deoxrSpace::Ref &GetSpaceView() const{ return pSpaceView; }
	inline const deoxrSpace::Ref &GetSpaceLocal() const{ return pSpaceLocal; }

	/** Left eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainLeftEye() const{ return pSwapchainLeftEye; }
	
	/** Right eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainRightEye() const{ return pSwapchainRightEye; }
	
	/** Depth left eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainDepthLeftEye() const{ return pSwapchainDepthLeftEye; }
	
	/** Depth right eye swapchain. */
	inline const deoxrSwapchain::Ref &GetSwapchainDepthRightEye() const{ return pSwapchainDepthRightEye; }
	
	/** Left eye pose. */
	inline const XrPosef &GetLeftEyePose() const{ return pLeftEyePose; }
	
	/** Left eye fov. */
	inline const XrFovf &GetLeftEyeFov() const{ return pLeftEyeFov; }
	
	/** Right eye pose. */
	inline const XrPosef &GetRightEyePose() const{ return pRightEyePose; }
	
	/** Right eye fov. */
	inline const XrFovf &GetRightEyeFov() const{ return pRightEyeFov; }
	
	/** Left eye hidden mesh or null. */
	inline deoxrHiddenMesh *GetLeftEyeHiddenMesh() const{ return pLeftEyeHiddenMesh; }
	
	/** Right eye hidden mesh or nullptr. */
	inline deoxrHiddenMesh *GetRightEyeHiddenMesh() const{ return pRightEyeHiddenMesh; }
	
	/** Update left eye hidden mesh. */
	void UpdateLeftEyeHiddenMesh();
	
	/** Update right eye hidden mesh. */
	void UpdateRightEyeHiddenMesh();
	
	/** Head position in stage coordinate system. */
	inline const decVector &GetHeadPosition() const{ return pHeadPosition; }
	
	/** Head orientation in stage coordinate system. */
	inline const decQuaternion &GetHeadOrientation() const{ return pHeadOrientation; }
	
	/** Head linear velocity in stage coordinate system. */
	inline const decVector &GetHeadLinearVelocity() const{ return pHeadLinearVelocity; }
	
	/** Head angular velocity in stage coordinate system. */
	inline const decVector &GetHeadAngularVelocity() const{ return pHeadAngularVelocity; }
	
	/** Left eye matrix relative to head. */
	inline const decMatrix &GetLeftEyeMatrix() const{ return pLeftEyeMatrix; }
	
	/** Right eye matrix relative to head. */
	inline const decMatrix &GetRightEyeMatrix() const{ return pRightEyeMatrix; }
	
	/** Graphic connection is OpenGL. */
	inline bool GetIsGACOpenGL() const{ return pIsGACOpenGL; }
	
	/** Restore OpenGL current. */
	void RestoreOpenGLCurrent();
	
	/** Swapchain format is supported. */
	bool HasSwapchainFormat( eSwapchainFormats format ) const;
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnumSwapchainFormats();
};

#endif


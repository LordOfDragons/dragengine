/* 
 * Drag[en]gine OpenXR
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

#ifndef _DEOXRSESSION_H_
#define _DEOXRSESSION_H_

#include "deoxrBasics.h"
#include "deoxrSpace.h"
#include "deoxrSwapchain.h"
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
	
	int64_t *pSwapchainFormats;
	int pSwapchainFormatCount;
	
	deoxrSwapchain::Ref pSwapchainLeftEye;
	deoxrSwapchain::Ref pSwapchainRightEye;
	
	XrPosef pLeftEyePose;
	XrFovf pLeftEyeFov;
	
	XrPosef pRightEyePose;
	XrFovf pRightEyeFov;
	
	decVector pHeadPosition;
	decQuaternion pHeadOrientation;
	decVector pHeadLinearVelocity;
	decVector pHeadAngularVelocity;
	decMatrix pLeftEyeMatrix;
	decMatrix pRightEyeMatrix;
	
	// graphic api connection
	bool pIsGACOpenGL;
	Display *pGACOpenGLDisplay;
	GLXDrawable pGACOpenGLDrawable;
	GLXContext pGACOpenGLContext;
	
	
	
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
	
	/** Begin session. */
	void Begin();
	
	/** End session. */
	void End();
	
	/** Attach action set. */
	void AttachActionSet( deoxrActionSet *actionSet );
	
	/** Begin frame. */
	void BeginFrame();
	
	/** End frame. */
	void EndFrame();
	
	/** Frame is running. */
	inline bool GetFrameRunning() const{ return pFrameRunning; }
	
	/** Runtime request rendering. */
	inline bool GetShouldRender() const{ return pShouldRender; }
	
	/** Sync actions. */
	void SyncActions();
	
	/** Space. */
	inline deoxrSpace *GetSpace() const{ return pSpaceStage; }
	
	/** Left eye swapchain. */
	inline deoxrSwapchain *GetSwapchainLeftEye() const{ return pSwapchainLeftEye; }
	
	/** Right eye swapchain. */
	inline deoxrSwapchain *GetSwapchainRightEye() const{ return pSwapchainRightEye; }
	
	/** Left eye pose. */
	inline const XrPosef &GetLeftEyePose() const{ return pLeftEyePose; }
	
	/** Left eye fov. */
	inline const XrFovf &GetLeftEyeFov() const{ return pLeftEyeFov; }
	
	/** Right eye pose. */
	inline const XrPosef &GetRightEyePose() const{ return pRightEyePose; }
	
	/** Right eye fov. */
	inline const XrFovf &GetRightEyeFov() const{ return pRightEyeFov; }
	
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
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pEnumSwapchainFormats();
};

#endif


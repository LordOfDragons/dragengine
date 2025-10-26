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

#ifndef _DEOGLVR_H_
#define _DEOGLVR_H_

#include "deoglVREye.h"
#include "../deoglBasics.h"
#include "../model/deoglRModel.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decTimeHistory.h>
#include <dragengine/resources/model/deModel.h>

class deoglRCamera;


/**
 * VR Support.
 */
class deoglVR{
private:
	enum eState{
		esBeginFrame,
		esWaitBeginFrameFinished,
		esRender,
		esSubmit
	};
	
	deoglRCamera &pCamera;
	
	deoglVREye pLeftEye, pRightEye;
	
	float pFovX, pFovY, pCameraFov, pCameraFovRatio, pPassthroughTransparency;
	bool pPassthroughEnabled;
	
	eState pState;
	
	decTimeHistory pTimeHistoryFrame;
	int pTargetFPS;
	float pTargetFPSHysteresis;
	
	bool pUseRenderStereo;
	decPoint pRenderStereoSize;
	deoglFramebuffer *pFBOStereo;
	
	decPoint pDebugPanelSize;
	deoglRenderTarget::Ref pRenderTargetDebugPanel;
	decVector2 pDebugPanelRenderSize;
	decColorMatrix pDebugPanelColorTransform;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create VR support. */
	deoglVR( deoglRCamera &camera );
	
	/** Clean up VR support. */
	~deoglVR();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Camera owning the VR. */
	inline deoglRCamera &GetCamera() const{ return pCamera; }
	
	/** Left eye. */
	inline deoglVREye &GetLeftEye(){ return pLeftEye; }
	inline const deoglVREye &GetLeftEye() const{ return pLeftEye; }
	
	/** Right eye. */
	inline deoglVREye &GetRightEye(){ return pRightEye; }
	inline const deoglVREye &GetRightEye() const{ return pRightEye; }
	
	/** Horizontal field of view. */
	inline float GetFovX() const{ return pFovX; }
	
	/** Vertical field of view. */
	inline float GetFovY() const{ return pFovY; }
	
	/** Camera field of view. */
	inline float GetCameraFov() const{ return pCameraFov; }
	
	/** Camera field of view ratio. */
	inline float GetCameraFovRatio() const{ return pCameraFovRatio; }
	
	/** Passthrough transparency. */
	inline float GetPassthroughTransparency() const{ return pPassthroughTransparency; }
	
	/** Passthrough enabled. */
	inline bool GetPassthroughEnabled() const{ return pPassthroughEnabled; }
	
	
	/** Update target frame rate. */
	void UpdateTargetFPS( float elapsed );
	
	/** Target frame rate. */
	inline int GetTargetFPS() const{ return pTargetFPS; }
	
	/** Use render stereo rendering. */
	inline bool GetUseRenderStereo() const{ return pUseRenderStereo; }
	
	/** Drop stereo framebuffer. */
	void DropFBOStereo();
	
	
	
	/** Debug panel size. */
	inline const decPoint &GetDebugPanelSize() const{ return pDebugPanelSize; }
	
	/** Get debug panel render target. Creates it if not created yet. */
	const deoglRenderTarget::Ref &GetRenderTargetDebugPanel();
	
	/** Debug panel render size. */
	inline const decVector2 &GetDebugPanelRenderSize() const{ return pDebugPanelRenderSize; }
	
	/** Debug panel color transform. */
	inline const decColorMatrix &GetDebugPanelColorTransform() const{ return pDebugPanelColorTransform; }
	
	
	
	/** Start begin frame. */
	void StartBeginFrame();
	
	/** Wait until begin frame finished. */
	void WaitBeginFrameFinished();
	
	/** Render if required. */
	void Render();
	
	/** Submit if required. */
	void Submit();
	
	/** End frame. */
	void EndFrame();
	/*@}*/
	
	
	
private:
	void pGetParameters();
	int pCalcTargetFPS( float frameTime ) const;
	void pRenderStereo();
};

#endif

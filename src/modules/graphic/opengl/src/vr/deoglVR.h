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
		esRender,
		esSubmit
	};
	
	deoglRCamera &pCamera;
	
	deoglVREye pLeftEye;
	deoglVREye pRightEye;
	
	float pFovX;
	float pFovY;
	float pCameraFov;
	float pCameraFovRatio;
	
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
	
	
	
	/** Begin frame. */
	void BeginFrame();
	
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

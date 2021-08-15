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

#include "../target/deoglRenderTarget.h"

#include <dragengine/common/math/decMath.h>

class deoglRCamera;


/**
 * VR Support.
 */
class deoglVR{
public:
	struct sProjection{
		double left;
		double right;
		double top;
		double bottom;
	};
	
	
	
private:
	enum eState{
		esRender,
		esSubmit
	};
	
	deoglRCamera &pCamera;
	
	decPoint pRenderSize;
	sProjection pProjectionLeftEye;
	sProjection pProjectionRightEye;
	decDMatrix pMatrixViewToLeftEye;
	decDMatrix pMatrixViewToRightEye;
	float pFovX;
	float pFovY;
	float pCameraFov;
	float pCameraFovRatio;
	
	deoglRenderTarget::Ref pTargetLeftEye;
	deoglRenderTarget::Ref pTargetRightEye;
	decVector2 pCanvasTCFromLeftEye;
	decVector2 pCanvasTCToLeftEye;
	decVector2 pCanvasTCFromRightEye;
	decVector2 pCanvasTCToRightEye;
	eState pState;
	
	
	
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
	
	/** Render size. */
	inline const decPoint &GetRenderSize() const{ return pRenderSize; }
	
	/** Projection parameters for left eye. */
	inline const sProjection &GetProjectionLeftEye() const{ return pProjectionLeftEye; }
	
	/** Projection parameters for right eye. */
	inline const sProjection &GetProjectionRightEye() const{ return pProjectionRightEye; }
	
	/** Matrix transforming from view space to left eye space. */
	inline const decDMatrix &GetMatrixViewToLeftEye() const{ return pMatrixViewToLeftEye; }
	
	/** Matrix transforming from view space to right eye space. */
	inline const decDMatrix &GetMatrixViewToRightEye() const{ return pMatrixViewToRightEye; }
	
	/** Horizontal field of view. */
	inline float GetFovX() const{ return pFovX; }
	
	/** Vertical field of view. */
	inline float GetFovY() const{ return pFovY; }
	
	/** Camera field of view. */
	inline float GetCameraFov() const{ return pCameraFov; }
	
	/** Camera field of view ratio. */
	inline float GetCameraFovRatio() const{ return pCameraFovRatio; }
	
	/**
	 * Create projection matrix matching depth usage mode. Depending on the inverse depth
	 * mode used the projection matrix is either infinite or non-infinite.
	 */
	decDMatrix CreateProjectionDMatrix( const sProjection &projection, float znear, float zfar ) const;
	
	/**
	 * Create frustum matrix. This is the same as CreateProjectionDMatrix but always
	 * creates a non-infinite projection matrix.
	 */
	decDMatrix CreateFrustumDMatrix( const sProjection &projection, float znear, float zfar ) const;
	
	
	
	/** Left eye render target. */
	inline deoglRenderTarget *GetTargetLeftEye() const{ return pTargetLeftEye; }
	
	/** Right eye render target. */
	inline deoglRenderTarget *GetTargetRightEye() const{ return pTargetRightEye; }
	
	/** Texture coordinates to use to render from left eye render target to canvas. */
	inline const decVector2 &GetCanvasTCFromLeftEye() const{ return pCanvasTCFromLeftEye; }
	inline const decVector2 &GetCanvasTCToLeftEye() const{ return pCanvasTCToLeftEye; }
	
	/** Texture coordinates to use to render from right eye render target to canvas. */
	inline const decVector2 &GetCanvasTCFromRightEye() const{ return pCanvasTCFromRightEye; }
	inline const decVector2 &GetCanvasTCToRightEye() const{ return pCanvasTCToRightEye; }
	
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
	void pCleanUp();
	void pGetParameters( deoglRenderThread &renderThread );
	void pRenderLeftEye( deoglRenderThread &renderThread, const decPoint &size );
	void pRenderRightEye( deoglRenderThread &renderThread, const decPoint &size );
};

#endif

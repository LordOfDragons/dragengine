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

#ifndef _DEOGLVREYE_H_
#define _DEOGLVREYE_H_

#include "../deoglBasics.h"
#include "../model/deoglRModel.h"
#include "../target/deoglRenderTarget.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>

class deoglVR;
class deBaseVRModule;


/**
 * VR Support.
 */
class deoglVREye{
public:
	struct sViewImage {
		deoglFramebuffer *fbo;
		GLuint texture;
	};
	
	
	
private:
	deoglVR &pVR;
	deBaseVRModule::eEye pEye;
	
	decPoint pTargetSize;
	decPoint pRenderSize;
	deBaseVRModule::eVRRenderFormat pRenderFormat;
	
	double pProjectionLeft;
	double pProjectionRight;
	double pProjectionTop;
	double pProjectionBottom;
	
	decMatrix pMatrixViewToEye;
	deModel::Ref pHiddenMesh;
	deoglRModel::Ref pHiddenRMesh;
	
	deoglRenderTarget::Ref pRenderTarget;
	decVector2 pCanvasTCFrom;
	decVector2 pCanvasTCTo;
	
	GLuint *pVRGetViewsBuffer;
	int pVRGetViewsBufferSize;
	
	sViewImage *pVRViewImages;
	int pVRViewImageCount;
	decVector2 pVRViewTCFrom;
	decVector2 pVRViewTCTo;
	int pAcquiredVRViewImage;
	
	bool pUseGammaCorrection;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create VR eye. */
	deoglVREye( deoglVR &vr, deBaseVRModule::eEye eye );
	
	/** Clean up VR eye. */
	~deoglVREye();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** VR. */
	inline deoglVR &GetVR() const{ return pVR; }
	
	/** Target size. */
	inline const decPoint &GetTargetSize() const{ return pTargetSize; }
	
	/** Projection. */
	inline double GetProjectionLeft() const{ return pProjectionLeft; }
	inline double GetProjectionRight() const{ return pProjectionRight; }
	inline double GetProjectionTop() const{ return pProjectionTop; }
	inline double GetProjectionBottom() const{ return pProjectionBottom; }
	
	/** Matrix transforming from view space to eye space. */
	inline const decMatrix &GetMatrixViewToEye() const{ return pMatrixViewToEye; }
	
	/** Hidden area mesh or nullptr. */
	inline deoglRModel *GetHiddenMesh() const{ return pHiddenRMesh; }
	
	/** Render target. */
	inline deoglRenderTarget *GetRenderTarget() const{ return pRenderTarget; }
	
	/** Texture coordinates to use to render from render target to canvas. */
	inline const decVector2 &GetCanvasTCFrom() const{ return pCanvasTCFrom; }
	inline const decVector2 &GetCanvasTCTo() const{ return pCanvasTCTo; }
	
	/** Use gamma correction. */
	inline bool GetUseGammaCorrection() const{ return pUseGammaCorrection; }
	
	/**
	 * Create projection matrix matching depth usage mode. Depending on the inverse depth
	 * mode used the projection matrix is either infinite or non-infinite.
	 */
	decDMatrix CreateProjectionDMatrix( float znear, float zfar ) const;
	
	/**
	 * Create frustum matrix. This is the same as CreateProjectionDMatrix but always
	 * creates a non-infinite projection matrix.
	 */
	decDMatrix CreateFrustumDMatrix( float znear, float zfar ) const;
	
	/** Begin frame. */
	void BeginFrame( deBaseVRModule &vrmodule );
	
	/** Render if required. */
	void Render();
	
	/** Submit if required. */
	void Submit( deBaseVRModule &vrmodule );
	/*@}*/
	
	
	
private:
	void pGetParameters( deBaseVRModule &vrmodule );
	void pLogParameters( deoglRenderThread &renderThread );
	void pUpdateEyeViews( deBaseVRModule &vrmodule );
	void pDestroyEyeViews();
	void pRender( deoglRenderThread &renderThread );
	const char *LogPrefix() const;
};

#endif

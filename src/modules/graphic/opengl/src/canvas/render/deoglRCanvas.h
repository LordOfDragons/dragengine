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

#ifndef _DEOGLRCANVAS_H_
#define _DEOGLRCANVAS_H_

#include "../../deoglGL.h"

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include "deoglRenderTarget.h"

class deoglRenderCanvasContext;
class deoglRenderThread;
class deoglRenderPlanMasked;


/**
 * Render canvas.
 */
class deoglRCanvas : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRCanvas> Ref;


public:
	static const int BlendModeCount = deCanvas::ebmAdd + 1;
	
	
	
private:
	deoglRenderThread &pRenderThread;
	decVector2 pPosition;
	decVector2 pSize;
	decTexMatrix2 pTransform;
	decColorMatrix pColorTransform;
	float pOrder;
	float pTransparency;
	deCanvas::eBlendModes pBlendMode;
	deoglRCanvas::Ref pMask;
	bool pVisible;
	deoglRenderTarget::Ref pMaskRenderTarget;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvas(deoglRenderThread &renderThread);
	
	/** Clean up peer. */
	virtual ~deoglRCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Position. */
	inline const decVector2 &GetPosition() const{ return pPosition; }
	
	/** Set position. */
	void SetPosition(const decVector2 &position);
	
	/** Size. */
	inline const decVector2 &GetSize() const{ return pSize; }
	
	/** Set size. */
	virtual void SetSize(const decVector2 &size);
	
	/** Transformation matrix. */
	inline const decTexMatrix2 &GetTransform() const{ return pTransform; }
	
	/** Set transformation matrix. */
	void SetTransform(const decTexMatrix2 &transform);
	
	/** Color transformation matrix. */
	inline const decColorMatrix &GetColorTransform() const{ return pColorTransform; }
	
	/** Set color transformation matrix. */
	void SetColorTransform(const decColorMatrix &transform);
	
	/** Order used for sorting. */
	inline float GetOrder() const{ return pOrder; }
	
	/** Set order. */
	void SetOrder(float order);
	
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency. */
	void SetTransparency(float transparency);
	
	/** Blend mode. */
	inline deCanvas::eBlendModes GetBlendMode() const{ return pBlendMode; }
	
	/** Set blend mode. */
	void SetBlendMode(deCanvas::eBlendModes mode);
	
	/** Mask. */
	inline const deoglRCanvas::Ref &GetMask() const{ return pMask; }
	
	/** Set mask. */
	void SetMask(deoglRCanvas *mask);
	
	/** Visible. */
	inline bool GetVisible() const{ return pVisible; }
	
	/** Set visible. */
	void SetVisible(bool visible);
	
	/** Mask render target or \em NULL if not ready. */
	inline const deoglRenderTarget::Ref &GetMaskRenderTarget() const{ return pMaskRenderTarget; }
	
	/** Dirty mask render target if present. */
	void DirtyMaskRenderTarget();
	
	
	
	/** Prepare for rendering. */
	virtual void PrepareForRender(const deoglRenderPlanMasked *renderPlanMask);
	
	/** Prepare for rendering render. */
	virtual void PrepareForRenderRender(const deoglRenderPlanMasked *renderPlanMask);
	
	/** Render. */
	virtual void Render(const deoglRenderCanvasContext &context) = 0;
	/*@}*/
};

#endif

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

#ifndef _DEOGLREFFECTOVERLAYIMAGE_H_
#define _DEOGLREFFECTOVERLAYIMAGE_H_

#include "deoglREffect.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../texture/deoglRImage.h"

#include <dragengine/common/math/decMath.h>


/**
 * Render effect overlay image.
 */
class deoglREffectOverlayImage : public deoglREffect{
private:
	float pTransparency;
	deoglRImage::Ref pImage;
	
	const deoglPipeline *pPipeline;
	const deoglPipeline *pPipelineStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render effect. */
	deoglREffectOverlayImage(deoglRenderThread &renderThread);
	
	/** Clean up render effect. */
	virtual ~deoglREffectOverlayImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Transparency. */
	inline float GetTransparency() const{ return pTransparency; }
	
	/** Set transparency. */
	void SetTransparency(float transparency);
	
	/** Image or nullptr to render nothing. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Set image or nullptr to render nothing. */
	void SetImage(deoglRImage *image);
	
	
	
	/** Get pipeline creating it if required. */
	const deoglPipeline *GetPipeline();
	const deoglPipeline *GetPipelineStereo();
	
	/** Prepare for render. */
	virtual void PrepareForRender();
	
	/** Render effect. */
	virtual void Render(deoglRenderPlan &plan);
};

#endif

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

#ifndef _DEOGLREFFECTDISTORTIMAGE_H_
#define _DEOGLREFFECTDISTORTIMAGE_H_

#include "deoglREffect.h"
#include "../../pipeline/deoglPipeline.h"
#include "../../texture/deoglRImage.h"

#include <dragengine/common/math/decMath.h>


/**
 * Render effect distort image.
 */
class deoglREffectDistortImage : public deoglREffect{
private:
	decVector2 pStrength;
	deoglRImage::Ref pImage;
	
	const deoglPipeline *pPipeline;
	const deoglPipeline *pPipelineStereo;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render effect. */
	deoglREffectDistortImage( deoglRenderThread &renderThread );
	
	/** Clean up render effect. */
	virtual ~deoglREffectDistortImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Distortion strength in U and V direction. */
	inline const decVector2 &GetStrength() const{ return pStrength; }
	
	/** Set distortion strength in U and V direction. */
	void SetStrength( const decVector2 &strength );
	
	/** Image or nullptr to render nothing. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Set image or nullptr to render nothing. */
	void SetImage( deoglRImage *image );
	
	
	
	/** Get pipeline creating it if required. */
	const deoglPipeline *GetPipeline();
	const deoglPipeline *GetPipelineStereo();
	
	/** Prepare for render. */
	virtual void PrepareForRender();
	
	/** Render effect. */
	virtual void Render( deoglRenderPlan &plan );
};

#endif

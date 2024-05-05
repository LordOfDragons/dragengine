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

#ifndef _DEOGLRCANVASIMAGE_H_
#define _DEOGLRCANVASIMAGE_H_

#include "deoglRCanvas.h"
#include "../../texture/deoglRImage.h"


/**
 * Render canvas image.
 */
class deoglRCanvasImage : public deoglRCanvas{
private:
	deoglRImage::Ref pImage;
	decTexMatrix2 pTCTransform;
	decVector2 pTCClampMin;
	decVector2 pTCClampMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCanvasImage( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCanvasImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Image or nullptr if not set. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Set image or nullptr if not set. */
	void SetImage( deoglRImage *image );
	
	/** Texture coordinate transformation matrix. */
	inline const decTexMatrix2 &GetTCTransform() const{ return pTCTransform; }
	
	/** Set texture coordinate transformation matrix. */
	void SetTCTransform( const decTexMatrix2 &transform );
	
	/** Texture coordinates clamp minimum. */
	inline const decVector2 &GetTCClampMinimum() const{ return pTCClampMin; }
	
	/** Set texture coordinates clamp minimum. */
	void SetTCClampMinimum( const decVector2 &clamp );
	
	/** Texture coordinates clamp maximum. */
	inline const decVector2 &GetTCClampMaximum() const{ return pTCClampMax; }
	
	/** Set texture coordinates clamp maximum. */
	void SetTCClampMaximum( const decVector2 &clamp );
	
	
	
	/** Prepare for rendering. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/** Render. */
	virtual void Render( const deoglRenderCanvasContext &context );
	/*@}*/
};

#endif

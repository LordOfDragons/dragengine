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

#ifndef _DEOGLRRDSRENDERABLEIMAGE_H_
#define _DEOGLRRDSRENDERABLEIMAGE_H_

#include "deoglRDSRenderable.h"
#include "../../../../texture/deoglRImage.h"

#include <dragengine/deObject.h>


/**
 * Render dynamic skin image renderable.
 */
class deoglRDSRenderableImage : public deoglRDSRenderable{
private:
	deoglRImage::Ref pImage;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create render dynamic skin image renderable. */
	deoglRDSRenderableImage( deoglRDynamicSkin &dynamicSkin );
	
	/** Clean up render dynamic skin image renderable. */
	virtual ~deoglRDSRenderableImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Image. */
	inline const deoglRImage::Ref &GetImage() const{ return pImage; }
	
	/** Set image. */
	void SetImage( deoglRImage *image );
	
	/** Prepare for render. */
	virtual void PrepareForRender( const deoglRenderPlanMasked *renderPlanMask );
	
	/**
	 * Get texture to use for rendering or \em NULL if not applicable.
	 * \details Default implementation returns \em NULL.
	 */
	virtual deoglTexture *GetRenderTexture();
	/*@}*/
};

#endif

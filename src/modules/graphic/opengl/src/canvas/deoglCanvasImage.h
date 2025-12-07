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

#ifndef _DEOGLCANVASIMAGE_H_
#define _DEOGLCANVASIMAGE_H_

#include "deoglCanvas.h"

class deoglRCanvasImage;
class deoglImage;
class deCanvasImage;


/**
 * Canvas peer.
 */
class deoglCanvasImage : public deoglCanvas{
private:
	deCanvasImage &pCanvasImage;
	deoglRCanvasImage *pRCanvasImage;
	deoglImage *pImage;
	bool pDirty;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCanvasImage(deGraphicOpenGl &ogl, deCanvasImage &canvas);
	
	/** Clean up peer. */
	~deoglCanvasImage() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Drop render canvas if not \em NULL. */
	void DropRCanvas() override;
	
	/**
	 * Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	void SyncContentToRender() override;
	
	/** Image requires sync. */
	void ImageRequiresSync();
	
	/** Drop image. */
	void DropImage();
	
	/** Content changed. */
	void ContentChanged() override;
	/*@}*/
	
	
	
protected:
	/** Create render canvas. Subclass responsibility. */
	deoglRCanvas *CreateRCanvas() override;
};

#endif

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

#ifndef _DEDSRENDERABLECANVAS_H_
#define _DEDSRENDERABLECANVAS_H_

#include "deDSRenderable.h"
#include "../../../canvas/deCanvasView.h"


/**
 * \brief Dynamic skin renderable canvas.
 *
 * The texture is defined using the content of a temporary render target.
 * Temporary render targets are not created by the user but by the Graphic
 * Module on the fly if required and for as long as it is required. The
 * user only specifies the content using a deCanvasView. This allows the
 * Graphic Module to use GPU memory efficiently. In general the size of
 * the canvas view defines the size of the used render target but the
 * Graphic Module is free to choose a fitting size itself.
 */
class DE_DLL_EXPORT deDSRenderableCanvas : public deDSRenderable{
private:
	int pComponentCount;
	int pBitCount;
	deCanvasView::Ref pCanvas;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderable. */
	deDSRenderableCanvas( const char *name );
	
	/** \brief Clean up renderable. */
	virtual ~deDSRenderableCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/**
	 * \brief Set number of components.
	 * \throws deeInvalidParam \em componentCount is less than 1.
	 * \throws deeInvalidParam \em componentCount is greater than 4.
	 */
	void SetComponentCount( int componentCount );
	
	/** \brief Bit count of each component. */
	inline int GetBitCount() const{ return pBitCount; }
	
	/**
	 * \brief Set bit count of each component.
	 * \throws deeInvalidParam \em bitCount is not 8, 16 or 32.
	 */
	void SetBitCount( int bitCount );
	
	/** \brief Canvas view or NULL if not set. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	/** \brief Set canvas view or NULL if not set. */
	void SetCanvas( deCanvasView *canvas );
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit the renderable. */
	virtual void Visit( deDSRenderableVisitor &visitor );
};

#endif

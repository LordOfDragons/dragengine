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

#ifndef _DEOGLRCAPTURECANVAS_H_
#define _DEOGLRCAPTURECANVAS_H_

#include "../../texture/pixelbuffer/deoglPixelBuffer.h"

#include <dragengine/deObject.h>

class deoglRRenderWindow;
class deoglRCanvasView;
class deoglRenderThread;



/**
 * Render capture canvas.
 */
class deoglRCaptureCanvas : public deObject{
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoglRCaptureCanvas> Ref;


private:
	deoglRenderThread &pRenderThread;
	deoglRCanvasView *pCanvasView;
	deoglPixelBuffer::Ref pPixelBuffer;
	bool pCapturePending;
	int pComponentCount;
	int pBitCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglRCaptureCanvas( deoglRenderThread &renderThread );
	
	/** Clean up peer. */
	virtual ~deoglRCaptureCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** Canvas view. */
	inline deoglRCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** Set canvas view. */
	void SetCanvasView( deoglRCanvasView *canvasView );
	
	/** Pixel buffer or \em NULL if no capture is pending. */
	inline const deoglPixelBuffer::Ref &GetPixelBuffer() const{ return pPixelBuffer; }
	
	/** Create pixel buffer. */
	void StartCapture( int width, int height, int componentCount, int bitCount );
	
	/** Drop pixel buffer. */
	void DropPixelBuffer();
	
	/** Capture is pending. */
	inline bool GetCapturePending() const{ return pCapturePending; }
	
	/** Set capture is pending. */
	void SetCapturePending( bool capturePending );
	
	
	
	/** Capture render window canvas view required. */
	void CaptureRenderWindow( deoglRRenderWindow &renderWindow );
	
	/** Capture canvas view not linked to a render window. */
	void CapturePending();
	
	/** Set capture to black. */
	void CaptureBlack();
	/*@}*/
};

#endif

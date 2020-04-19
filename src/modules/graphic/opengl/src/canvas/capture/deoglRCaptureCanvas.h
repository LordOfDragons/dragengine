/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
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

#ifndef _DEOGLRCAPTURECANVAS_H_
#define _DEOGLRCAPTURECANVAS_H_

#include <dragengine/deObject.h>

class deoglRRenderWindow;
class deoglPixelBuffer;
class deoglRCanvasView;
class deoglRenderThread;



/**
 * \brief Render capture canvas.
 */
class deoglRCaptureCanvas : public deObject{
private:
	deoglRenderThread &pRenderThread;
	deoglRCanvasView *pCanvasView;
	deoglPixelBuffer *pPixelBuffer;
	bool pCapturePending;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglRCaptureCanvas( deoglRenderThread &renderThread );
	
	/** \brief Clean up peer. */
	virtual ~deoglRCaptureCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render thread. */
	inline deoglRenderThread &GetRenderThread() const{ return pRenderThread; }
	
	/** \brief Canvas view. */
	inline deoglRCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Set canvas view. */
	void SetCanvasView( deoglRCanvasView *canvasView );
	
	/** \brief Pixel buffer or \em NULL if no capture is pending. */
	inline deoglPixelBuffer *GetPixelBuffer() const{ return pPixelBuffer; }
	
	/** \brief Create pixel buffer. */
	void StartCapture( int width, int height, int componentCount, int bitCount );
	
	/** \brief Drop pixel buffer. */
	void DropPixelBuffer();
	
	/** \brief Capture is pending. */
	inline bool GetCapturePending() const{ return pCapturePending; }
	
	/** \brief Set capture is pending. */
	void SetCapturePending( bool capturePending );
	
	
	
	/** \brief Capture render window canvas view required. */
	void CaptureRenderWindow( deoglRRenderWindow &renderWindow );
	
	/** \brief Capture canvas view not linked to a render window. */
	void CapturePending();
	
	/** \brief Set capture to black. */
	void CaptureBlack();
	/*@}*/
};

#endif

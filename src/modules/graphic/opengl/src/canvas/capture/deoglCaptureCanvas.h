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

#ifndef _DEOGLCAPTURECANVAS_H_
#define _DEOGLCAPTURECANVAS_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicCaptureCanvas.h>
#include <dragengine/common/math/decMath.h>

class deoglRCaptureCanvas;
class deCaptureCanvas;
class deGraphicOpenGl;



/**
 * Capture canvas peer.
 */
class deoglCaptureCanvas : public deBaseGraphicCaptureCanvas{
private:
	deGraphicOpenGl &pOgl;
	deCaptureCanvas &pCaptureCanvas;
	
	deoglRCaptureCanvas *pRCaptureCanvas;
	
	bool pDirtyCanvasView;
	bool pCapturePending;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCaptureCanvas(deGraphicOpenGl &ogl, deCaptureCanvas &captureCanvas);
	
	/** Clean up peer. */
	virtual ~deoglCaptureCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** OpenGL module. */
	inline deGraphicOpenGl &GetOgl(){return pOgl;}
	inline const deGraphicOpenGl &GetOgl() const{return pOgl;}
	
	/** Capture canvas engine resource. */
	inline deCaptureCanvas &GetCaptureCanvas(){return pCaptureCanvas;}
	inline const deCaptureCanvas &GetCaptureCanvas() const{return pCaptureCanvas;}
	
	/** Render capture canvas or \em NULL if not set. */
	inline deoglRCaptureCanvas *GetRCaptureCanvas() const{return pRCaptureCanvas;}
	
	
	
	/**
	 * Update render thread counterpart if required.
	 * \details For finished captures the image is created from the pixel buffer and
	 *          stored for the game scripts to retrieve. For pending captures the
	 *          capture process is started.
	 */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** Canvas view changed. */
	virtual void CanvasViewChanged();
	
	/** Image changed. */
	virtual void ImageChanged();
	
	/** Capture changed. */
	virtual void CaptureChanged();
	/*@}*/
};

#endif

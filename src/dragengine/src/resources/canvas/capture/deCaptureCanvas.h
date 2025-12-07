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

#ifndef _DECAPTURECANVAS_H_
#define _DECAPTURECANVAS_H_

#include "../deCanvasView.h"
#include "../../deResource.h"
#include "../../image/deImage.h"

class deBaseGraphicCaptureCanvas;
class deCaptureCanvasManager;


/**
 * \brief Capture canvas.
 * 
 * Capture canvas allows to capture the rendering result of a canvas view into an
 * deImage resource. Capturing is done by the graphic module during the next frame
 * rendering. During the next deGraphicSystem::RenderWindows() call or at the end
 * of the current deGraphicSystem::RenderWindows() call the result is stored back
 * into the capture canvas. Whichever is the case depends on the rendering
 * method of the graphic module. You know when the result is ready \em capture is
 * set to false. The image resource is created by the caller and allows to
 * request the result in a specific size and format. To do a successful capturing
 * of a render view you have to do the following:
 * - set image to a valid deImage object.
 * - set \em capture to true.
 * - during the next one or two deGraphicSystem::RenderWindows() calls check if
 * \em capture is false.
 * You can capture the canvas view of a render window or any other canvas view
 * similar to a deCanvasCanvas.
 */
class DE_DLL_EXPORT deCaptureCanvas : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deCaptureCanvas> Ref;
	
	
private:
	deCanvasView::Ref pCanvasView;
	deImage::Ref pImage;
	bool pCapture;
	
	deBaseGraphicCaptureCanvas *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capture canvas. */
	deCaptureCanvas(deCaptureCanvasManager *manager);
	
protected:
	/**
	 * \brief Clean up capture canvas.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deCaptureCanvas();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Canvas view to capture. */
	inline deCanvasView *GetCanvasView() const{return pCanvasView;}
	
	/** \brief Set canvas view to capture. */
	void SetCanvasView(deCanvasView *canvasView);
	
	/** \brief Image to capture canvas view into. */
	inline deImage *GetImage() const{return pImage;}
	
	/** \brief Set image to capture canvas view into. */
	void SetImage(deImage *image);
	
	/** \brief Capture in progress. */
	inline bool GetCapture() const{return pCapture;}
	
	/** \brief Set if capture is in progress. */
	void SetCapture(bool capture);
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object or NULL if not set. */
	inline deBaseGraphicCaptureCanvas *GetPeerGraphic() const{return pPeerGraphic;}
	
	/** \brief Set graphic system peer object or NULL if not set. */
	void SetPeerGraphic(deBaseGraphicCaptureCanvas *peer);
	/*@}*/
};

#endif

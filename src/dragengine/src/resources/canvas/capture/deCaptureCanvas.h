/* 
 * Drag[en]gine Game Engine
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

#ifndef _DECAPTURECANVAS_H_
#define _DECAPTURECANVAS_H_

#include "../deCanvasViewReference.h"
#include "../../deResource.h"
#include "../../image/deImageReference.h"

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
	deCanvasViewReference pCanvasView;
	deImageReference pImage;
	bool pCapture;
	
	deBaseGraphicCaptureCanvas *pPeerGraphic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create capture canvas. */
	deCaptureCanvas( deCaptureCanvasManager *manager );
	
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
	inline deCanvasView *GetCanvasView() const{ return pCanvasView; }
	
	/** \brief Set canvas view to capture. */
	void SetCanvasView( deCanvasView *canvasView );
	
	/** \brief Image to capture canvas view into. */
	inline deImage *GetImage() const{ return pImage; }
	
	/** \brief Set image to capture canvas view into. */
	void SetImage( deImage *image );
	
	/** \brief Capture in progress. */
	inline bool GetCapture() const{ return pCapture; }
	
	/** \brief Set if capture is in progress. */
	void SetCapture( bool capture );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Graphic system peer object or NULL if not set. */
	inline deBaseGraphicCaptureCanvas *GetPeerGraphic() const{ return pPeerGraphic; }
	
	/** \brief Set graphic system peer object or NULL if not set. */
	void SetPeerGraphic( deBaseGraphicCaptureCanvas *peer );
	/*@}*/
};

#endif

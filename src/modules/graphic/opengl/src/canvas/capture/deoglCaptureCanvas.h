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

#ifndef _DEOGLCAPTURECANVAS_H_
#define _DEOGLCAPTURECANVAS_H_

#include <dragengine/systems/modules/graphic/deBaseGraphicCaptureCanvas.h>
#include <dragengine/common/math/decMath.h>

class deoglRCaptureCanvas;
class deCaptureCanvas;
class deGraphicOpenGl;



/**
 * \brief Capture canvas peer.
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
	/** \brief Create peer. */
	deoglCaptureCanvas( deGraphicOpenGl &ogl, deCaptureCanvas &captureCanvas );
	
	/** \brief Clean up peer. */
	virtual ~deoglCaptureCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenGL module. */
	inline deGraphicOpenGl &GetOgl(){ return pOgl; }
	inline const deGraphicOpenGl &GetOgl() const{ return pOgl; }
	
	/** \brief Capture canvas engine resource. */
	inline deCaptureCanvas &GetCaptureCanvas(){ return pCaptureCanvas; }
	inline const deCaptureCanvas &GetCaptureCanvas() const{ return pCaptureCanvas; }
	
	/** \brief Render capture canvas or \em NULL if not set. */
	inline deoglRCaptureCanvas *GetRCaptureCanvas() const{ return pRCaptureCanvas; }
	
	
	
	/**
	 * \brief Update render thread counterpart if required.
	 * \details For finished captures the image is created from the pixel buffer and
	 *          stored for the game scripts to retrieve. For pending captures the
	 *          capture process is started.
	 */
	void SyncToRender();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Canvas view changed. */
	virtual void CanvasViewChanged();
	
	/** \brief Image changed. */
	virtual void ImageChanged();
	
	/** \brief Capture changed. */
	virtual void CaptureChanged();
	/*@}*/
};

#endif

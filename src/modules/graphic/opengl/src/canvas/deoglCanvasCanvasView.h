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

#ifndef _DEOGLCANVASCANVASVIEW_H_
#define _DEOGLCANVASCANVASVIEW_H_

#include "deoglCanvas.h"
#include "deoglCanvasViewListener.h"

class deoglRCanvasCanvasView;

class deCanvasCanvasView;


/**
 * \brief Canvas view peer.
 */
class deoglCanvasCanvasView : public deoglCanvas, deoglCanvasViewListener{
private:
	deCanvasCanvasView &pCanvasCanvasView;
	deoglRCanvasCanvasView *pRCanvasCanvasView;
	deoglCanvasView *pCanvasView;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglCanvasCanvasView( deGraphicOpenGl &ogl, deCanvasCanvasView &canvas );
	
	/** \brief Clean up peer. */
	virtual ~deoglCanvasCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/**
	 * \brief Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
	
	/** Canvas view has been destroyed. Owner has to drop weak reference to canvas view. */
	virtual void CanvasViewDestroyed();
	
	/** Canvas view requires sync. */
	virtual void CanvasViewRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** \brief Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas();
};

#endif

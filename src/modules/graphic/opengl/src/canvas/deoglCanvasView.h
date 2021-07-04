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

#ifndef _DEOGLCANVASVIEW_H_
#define _DEOGLCANVASVIEW_H_

#include "deoglCanvas.h"

#include <dragengine/common/collection/decPointerSet.h>

class deoglRCanvasView;
class deCanvasView;
class deoglCanvasViewListener;


/**
 * \brief Canvas peer.
 */
class deoglCanvasView : public deoglCanvas{
private:
	deCanvasView &pCanvasView;
	deoglRCanvasView *pRCanvasView;
	
	bool pDirtyPaint;
	bool pResizeRenderTarget;
	
	decPointerSet pListeners;
	bool pSyncRequestSend;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoglCanvasView( deGraphicOpenGl &ogl, deCanvasView &canvas );
	
	/** \brief Clean up peer. */
	virtual ~deoglCanvasView();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Render canvas view or \em NULL if not existing. */
	inline deoglRCanvasView *GetRCanvasView() const{ return pRCanvasView; }
	
	/** \brief Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/** \brief Order of a child changed. */
	void ChildOrderChanged();
	
	/** \brief Paint dirty. */
	inline bool GetDirtyPaint() const{ return pDirtyPaint; }
	
	/** \brief Set paint dirty. */
	void SetDirtyPaint();
	
	/** \brief Update render thread counterpart if required. */
	virtual void SyncToRender();
	
	/**
	 * \brief Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
	/*@}*/
	
	
	
	/** \name Listeners */
	/*@{*/
	/** Add a listener. */
	void AddListener( deoglCanvasViewListener *listener );
	
	/** Remove listener if existing. */
	void RemoveListener( deoglCanvasViewListener *listener );
	
	/** Notify all canvas view has been destroyed. */
	void NotifyDestroyed();
	
	/** Notify all canvas view requires sync. */
	void NotifyRequiresSync();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/** \brief Transform changed. */
	virtual void TransformChanged();
	
	/** \brief Color transform changed. */
	virtual void ColorTransformChanged();
	
	/** \brief Visible changed. */
	virtual void VisibleChanged();
	
	/** \brief Render order changed. */
	virtual void OrderChanged();
	
	/** \brief Transparency changed. */
	virtual void TransparencyChanged();
	
	/** \brief Blend mode changed. */
	virtual void BlendModeChanged();
	
	/** \brief Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** \brief Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas();
	
	
	
private:
	void pRequiresSync();
};

#endif

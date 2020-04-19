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

#ifndef _DEOGLDSRENDERABLECANVAS_H_
#define _DEOGLDSRENDERABLECANVAS_H_

#include "deoglDSRenderable.h"

class deoglCanvasView;
class deoglRDSRenderableCanvas;

class deDSRenderableCanvas;



/**
 * \brief Dynamic skin canvas renderable.
 */
class deoglDSRenderableCanvas : public deoglDSRenderable{
public:
	const deDSRenderableCanvas &pRenderableCanvas;
	deoglRDSRenderableCanvas *pRRenderableCanvas;
	deoglCanvasView *pCanvasView;
	bool pDirty;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create renderable. */
	deoglDSRenderableCanvas( deoglDynamicSkin &dynamicSkin, const deDSRenderableCanvas &renderable );
	
	/** \brief Clean up peer. */
	virtual ~deoglDSRenderableCanvas();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Renderable. */
	inline const deDSRenderableCanvas &GetRenderableCanvas() const{ return pRenderableCanvas; }
	
	/** \brief Render renderable. */
	virtual deoglRDSRenderable *GetRRenderable() const;
	
	/** \brief Renderable changed. */
	virtual void RenderableChanged();
	
	/** \brief Update render thread counterpart if required. */
	virtual void SyncToRender();
	
	/** \brief Canvas view requires sync. */
	void CanvasViewRequiresSync();
	
	/** \brief Drop canvas view. */
	void DropCanvasView();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

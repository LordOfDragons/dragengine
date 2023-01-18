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

#ifndef _DEOGLCANVASIMAGE_H_
#define _DEOGLCANVASIMAGE_H_

#include "deoglCanvas.h"

class deoglRCanvasImage;
class deoglImage;
class deCanvasImage;


/**
 * Canvas peer.
 */
class deoglCanvasImage : public deoglCanvas{
private:
	deCanvasImage &pCanvasImage;
	deoglRCanvasImage *pRCanvasImage;
	deoglImage *pImage;
	bool pDirty;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create peer. */
	deoglCanvasImage( deGraphicOpenGl &ogl, deCanvasImage &canvas );
	
	/** Clean up peer. */
	virtual ~deoglCanvasImage();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Drop render canvas if not \em NULL. */
	virtual void DropRCanvas();
	
	/**
	 * Prepare content for render thread counterpart.
	 * \details Called if content is dirty.
	 */
	virtual void SyncContentToRender();
	
	/** Image requires sync. */
	void ImageRequiresSync();
	
	/** Drop image. */
	void DropImage();
	
	/** Content changed. */
	virtual void ContentChanged();
	/*@}*/
	
	
	
protected:
	/** Create render canvas. Subclass responsibility. */
	virtual deoglRCanvas *CreateRCanvas();
};

#endif

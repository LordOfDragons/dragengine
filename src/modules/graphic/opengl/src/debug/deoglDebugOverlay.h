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

#ifndef _DEOGLDEBUGOVERLAY_H_
#define _DEOGLDEBUGOVERLAY_H_

#include "deoglDebugOverlay.h"

#include <dragengine/common/collection/decPointerList.h>

class deCanvasView;
class deGraphicOpenGl;


/**
 * Debug overlay manager.
 */
class deoglDebugOverlay{
private:
	deGraphicOpenGl &pOgl;
	decPointerList pList1, pList2;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create debug overlay manager. */
	deoglDebugOverlay( deGraphicOpenGl &ogl );
	
	/** Clean up debug overlay manager. */
	~deoglDebugOverlay();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * Prepare debug overlay.
	 * 
	 * Aligns the children canvas and synchronizes.
	 */
	void PrepareOverlay( deCanvasView &canvasView );
	/*@}*/
	
	
	
private:
	void pSortViews( deCanvasView &canvasView );
	void pAlignViews();
};

#endif

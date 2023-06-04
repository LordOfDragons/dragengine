/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _MEINFOBUBBLE_H_
#define _MEINFOBUBBLE_H_

#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>

class meView3D;
class igdeWidget;


/**
 * Information bubble.
 */
class meInfoBubble : public deObject{
public:
	typedef deTObjectReference<meInfoBubble> Ref;
	
	enum ePlacement{
		epTopLeft,
		epTopRight,
		epBottomLeft,
		epBottomRight
	};
	
	
	
private:
	meView3D &pView;
	
	deCanvasView::Ref pCanvasBubble;
	deCanvasView::Ref pCanvasContent;
	deCanvasPaint::Ref pCanvasBorder;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create information bubble. */
	meInfoBubble( meView3D &view );
	
protected:
	/** \brief Clean up information bubble. */
	virtual ~meInfoBubble();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Content view. */
	inline const deCanvasView::Ref &GetCanvasContent() const{ return pCanvasContent; }
	
	/** \brief Show bubble at mouse position. */
	void ShowAt( const decPoint &position, ePlacement placement );
	
	/** \brief Hide bubble. */
	void Hide();
	/*@}*/
	
	
	
private:
	void pResize();
	decPoint pPlace( const decPoint &position, ePlacement placement );
};

#endif

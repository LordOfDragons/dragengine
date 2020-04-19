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

#ifndef _DECANVASVISITOR_H_
#define _DECANVASVISITOR_H_

class deCanvas;
class deCanvasImage;
class deCanvasPaint;
class deCanvasCanvasView;
class deCanvasRenderWorld;
class deCanvasText;
class deCanvasVideoPlayer;
class deCanvasView;


/**
 * \brief Canvas visitor.
 */
class deCanvasVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create canvas visitor. */
	deCanvasVisitor();
	
	/** \brief Clean up canvas visitor. */
	virtual ~deCanvasVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit canvas. */
	virtual void VisitCanvas( deCanvas &canvas );
	
	/** \brief Visit image canvas. */
	virtual void VisitImage( deCanvasImage &canvas );
	
	/** \brief Visit paint canvas. */
	virtual void VisitPaint( deCanvasPaint &canvas );
	
	/** \brief Visit canvas view canvas. */
	virtual void VisitCanvasView( deCanvasCanvasView &canvas );
	
	/** \brief Visit render world canvas. */
	virtual void VisitRenderWorld( deCanvasRenderWorld &canvas );
	
	/** \brief Visit text canvas. */
	virtual void VisitText( deCanvasText &canvas );
	
	/** \brief Visit video player canvas. */
	virtual void VisitVideoPlayer( deCanvasVideoPlayer &canvas );
	
	/** \brief Visit view canvas. */
	virtual void VisitView( deCanvasView &canvas );
	/*@}*/
};

#endif

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

#ifndef _DEDSRENDERABLEVISITOR_H_
#define _DEDSRENDERABLEVISITOR_H_

class deDSRenderable;
class deDSRenderableValue;
class deDSRenderableColor;
class deDSRenderableImage;
class deDSRenderableCanvas;
class deDSRenderableVideoFrame;
class deDSRenderableCamera;


/**
 * \brief Visitor for dynamic skin renderables.
 */
class DE_DLL_EXPORT deDSRenderableVisitor{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	deDSRenderableVisitor();
	
	/** \brief Clean up visitor. */
	virtual ~deDSRenderableVisitor();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit renderable. */
	virtual void VisitRenderable( deDSRenderable &renderable );
	
	/** \brief Visit value renderable. */
	virtual void VisitValue( deDSRenderableValue &renderable );
	
	/** \brief Visit color renderable. */
	virtual void VisitColor( deDSRenderableColor &renderable );
	
	/** \brief Visit image renderable. */
	virtual void VisitImage( deDSRenderableImage &renderable );
	
	/** \brief Visit canvas renderable. */
	virtual void VisitCanvas( deDSRenderableCanvas &renderable );
	
	/** \brief Visit video frame renderable. */
	virtual void VisitVideoFrame( deDSRenderableVideoFrame &renderable );
	
	/** \brief Visit camera renderable. */
	virtual void VisitCamera( deDSRenderableCamera &renderable );
	/*@}*/
};

#endif

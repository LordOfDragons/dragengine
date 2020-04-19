/* 
 * Drag[en]gine IGDE Skin Editor
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

#ifndef _SEVCIDRAGNODE_H_
#define _SEVCIDRAGNODE_H_

#include <deigde/gui/event/igdeMouseDragListener.h>
#include <deigde/undo/igdeUndoReference.h>

class seViewConstructedView;
class deCanvas;



/**
 * \brief Drag node interaction.
 */
class seVCIDragNode : public igdeMouseDragListener{
protected:
	enum eDragAction{
		edaNone,
		edaMoveNode,
		edaResizeTopLeft,
		edaResizeTopRight,
		edaResizeBottomLeft,
		edaResizeBottomRight,
		edaResizeTop,
		edaResizeBottom,
		edaResizeLeft,
		edaResizeRight,
		edaRotateTopLeft,
		edaRotateTopRight,
		edaRotateBottomLeft,
		edaRotateBottomRight,
		edaShearTop,
		edaShearBottom,
		edaShearLeft,
		edaShearRight
	};
	
	
	
private:
	seViewConstructedView &pView;
	eDragAction pDragAction;
	decTexMatrix2 pDragMatrix;
	igdeUndoReference pUndo;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create listener. */
	seVCIDragNode( seViewConstructedView &view );
	
	/** \brief Clean up listener. */
	virtual ~seVCIDragNode();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/**
	 * \brief Dragging begins.
	 * \returns true to start drag operation or false to abort without calling OnDragFinish.
	 */
	virtual bool OnDragBegin();
	
	/**
	 * \brief Update dragging.
	 */
	virtual void OnDragUpdate();
	
	/**
	 * \brief Dragging ends.
	 */
	virtual void OnDragFinish( bool cancelled );
	/*@}*/
	
	
	
protected:
	bool pInsideMarker( const deCanvas &canvas, const decPoint &position ) const;
	void pMove();
	void pRotate();
	void pResize();
	void pShear();
};

#endif

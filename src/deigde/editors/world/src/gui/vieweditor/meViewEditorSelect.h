/* 
 * Drag[en]gine IGDE World Editor
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

#ifndef _MEVIEWEDITORSELECT_H_
#define _MEVIEWEDITORSELECT_H_

#include "meViewEditorNavigation.h"
#include "../meInfoBubble.h"

#include <dragengine/resources/collider/deColliderReference.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>

class meCLSelect;
class meCLClosestElement;



/**
 * View editor for selecting scene elements.
 */
class meViewEditorSelect : public meViewEditorNavigation{
private:
	meCLSelect *pCLSelect;
	meCLClosestElement *pCLClosest;
	deColliderReference pColVol;
	
	deCanvasPaint::Ref pCanvasSelect;
	
	meInfoBubble::Ref pInfoBubble;
	deCanvasText::Ref pInfoBubbleText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create view editor. */
	meViewEditorSelect( meView3D &view );
	
	/** Clean up view editor. */
	virtual ~meViewEditorSelect();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Update rectangular selection. */
	void UpdateRectSelection();
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** View size changed. */
	virtual void OnResize();
	
	/** The left mouse button has been pressed. Return true if handled. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** The left mouse button has been released. Return true if handled. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** The right mouse button has been pressed. Return true if handled. */
	virtual void OnRightMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** The right mouse button has been released. Return true if handled. */
	virtual void OnRightMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	
	/** The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	virtual void OnMouseWheel( int steps, bool shift, bool control );
	
	/** The mouse left view. */
	virtual void OnMousLeave();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateInfoBubble( int x, int y );
};

#endif

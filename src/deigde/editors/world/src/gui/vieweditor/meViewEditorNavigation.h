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

#ifndef _MEVIEWEDITORNAVIGATION_H_
#define _MEVIEWEDITORNAVIGATION_H_

#include "meViewEditor.h"

#include <deigde/undo/igdeUndoReference.h>



/**
 * \brief View Editor Navigation.
 * 
 * Base class for view editors with navigation support. Navigation support is done whenever modifier buttons
 * are held while moving with the right mouse button pressed. For subclasses it is marked if navigation is
 * in progress. This way subclasses can do their own processing during mouse movement if no navigation is
 * in progress. Otherwise simply do nothing if the right mouse button is pressed during mouse move.
 */
class meViewEditorNavigation : public meViewEditor{
private:
	float pOldDist;
	float pOldZoom;
	decVector pOldOrientation;
	
	bool pNavigating;
	
	igdeUndoReference pUndoCameraMove;
	igdeUndoReference pUndoCameraRotate;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new view editor. */
	meViewEditorNavigation( meView3D &view );
	/** Cleans up the view editor. */
	virtual ~meViewEditorNavigation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Determines if navigation is in progress. */
	inline bool GetNavigating() const{ return pNavigating; }
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** The right mouse button has been pressed. Return true if handled. */
	virtual void OnRightMouseButtonPress( int x, int y, bool shift, bool control );
	/** The right mouse button has been released. Return true if handled. */
	virtual void OnRightMouseButtonRelease( int x, int y, bool shift, bool control );
	/** The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	/** The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	virtual void OnMouseWheel( int steps, bool shift, bool control );
	/*@}*/
};

#endif

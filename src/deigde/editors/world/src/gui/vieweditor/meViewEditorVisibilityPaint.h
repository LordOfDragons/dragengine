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

#ifndef _MEVIEWEDITORVISIBILITYPAINT_H_
#define _MEVIEWEDITORVISIBILITYPAINT_H_

#include "meViewEditorNavigation.h"

class meCLVisibilityPaint;



/**
 * @brief View Editor Visibility Paint.
 * View editor for visibility painting terrain.
 */
class meViewEditorVisibilityPaint : public meViewEditorNavigation{
private:
	meCLVisibilityPaint *pVisPainter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new view editor. */
	meViewEditorVisibilityPaint( meView3D &view );
	/** Cleans up the view editor. */
	virtual ~meViewEditorVisibilityPaint();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** The left mouse button has been pressed. Return true if handled. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	/** The left mouse button has been released. Return true if handled. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	/** The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	/*@}*/
};

#endif

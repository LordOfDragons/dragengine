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

#ifndef _MEVIEWEDITORHEIGHTPAINT_H_
#define _MEVIEWEDITORHEIGHTPAINT_H_

#include "meViewEditorNavigation.h"

class meCLHeightPaint;



/**
 * @brief View Editor Height Paint.
 * View editor for height painting terrain.
 */
class meViewEditorHeightPaint : public meViewEditorNavigation{
private:
	float pPainterElapsedTime;
	bool pPainting;
	decPoint pPaintPosition;
	
	meCLHeightPaint *pHeightPainter;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new view editor. */
	meViewEditorHeightPaint( meView3D &view );
	/** Cleans up the view editor. */
	virtual ~meViewEditorHeightPaint();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
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

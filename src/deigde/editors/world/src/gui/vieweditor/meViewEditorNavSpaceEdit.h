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

#ifndef _MEVIEWEDITORNAVSPACEEDIT_H_
#define _MEVIEWEDITORNAVSPACEEDIT_H_

#include "meViewEditorNavigation.h"

#include <dragengine/resources/collider/deColliderReference.h>

class meCLSelect;



/**
 * \brief View editor navigation space edit.
 */
class meViewEditorNavSpaceEdit : public meViewEditorNavigation{
private:
	deColliderReference pColVol;
	meCLSelect *pCLSelect;
	float pSelectDistance;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view editor. */
	meViewEditorNavSpaceEdit( meView3D &view );
	
	/** \brief Clean up view editor. */
	virtual ~meViewEditorNavSpaceEdit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	
	
	/** \name Events */
	/*@{*/
	/** \brief View size changed. */
	virtual void OnResize();
	
	/** \brief Left mouse button has been pressed. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** \brief Left mouse button has been released. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** \brief Mouse has been moved. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	
	/** \brief The mouse wheel has been used. Steps contains the number of steps up (positive) or down (negative). Return true if handled. */
	virtual void OnMouseWheel( int steps, bool shift, bool control );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

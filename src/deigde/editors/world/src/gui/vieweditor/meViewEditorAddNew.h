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

#ifndef _MEVIEWEDITORADDNEW_H_
#define _MEVIEWEDITORADDNEW_H_

#include "meViewEditorNavigation.h"

#include <deigde/undo/igdeUndoReference.h>

#include <dragengine/resources/collider/deColliderReference.h>

class meCLAddDecal;
class meCLClosestElement;
class meCLSnapPoint;
class meFilterObjectsByClass;



/**
 * \brief View editor add new element.
 */
class meViewEditorAddNew : public meViewEditorNavigation{
private:
	meCLClosestElement *pClosestElement;
	meCLSnapPoint *pCLSnapPoint;
	meFilterObjectsByClass *pFilterObjectsByClass;
	meCLAddDecal *pAddDecal;
	igdeUndoReference pUndoAddObject;
	igdeUndoReference pUndoAddNavSpace;
	deColliderReference pCLCollider;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create view editor. */
	meViewEditorAddNew( meView3D &view );
	
	/** \brief Clean up view editor. */
	virtual ~meViewEditorAddNew();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/*@}*/
	
	/** \name Events */
	/*@{*/
	/** \brief A key on the keyboard has been pressed. Return true if handled. */
	virtual bool OnKeyPress( deInputEvent::eKeyCodes key, bool shift, bool control );
	
	/** \brief The left mouse button has been pressed. Return true if handled. */
	virtual void OnLeftMouseButtonPress( int x, int y, bool shift, bool control );
	
	/** \brief The left mouse button has been released. Return true if handled. */
	virtual void OnLeftMouseButtonRelease( int x, int y, bool shift, bool control );
	
	/** \brief The mouse has been moved. Return true if handled. */
	virtual void OnMouseMove( int x, int y, bool shift, bool control );
	/*@}*/
	
	
	
private:
	void pUpdateUndo( bool shift, bool control );
};

#endif

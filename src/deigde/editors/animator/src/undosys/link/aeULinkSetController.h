/* 
 * Drag[en]gine IGDE Animator Editor
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

// include only once
#ifndef _AEULINKSETCONTROLLER_H_
#define _AEULINKSETCONTROLLER_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class aeLink;
class aeController;



/**
 * Undo Link Set Controller.
 *
 * Sets the controller of a link.
 */
class aeULinkSetController : public igdeUndo{
private:
	aeLink *pLink;
	
	aeController *pOldController;
	aeController *pNewController;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeULinkSetController( aeLink *link, aeController *newController );
protected:
	/** Clean up undo. */
	virtual ~aeULinkSetController();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	/** Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif

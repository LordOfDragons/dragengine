/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2022, Roland Plüss (roland@rptd.ch)
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


#ifndef _AEUCONTROLLERPASTE_H_
#define _AEUCONTROLLERPASTE_H_

#include <deigde/undo/igdeUndo.h>
#include "../../animator/aeAnimator.h"
#include "../../animator/controller/aeControllerList.h"



/**
 * Undo action paste controller.
 */
class aeUControllerPaste : public igdeUndo{
private:
	const aeAnimator::Ref pAnimator;
	aeControllerList pControllerList;
	int pIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create a new undo object. */
	aeUControllerPaste( aeAnimator *animator, const aeControllerList &controllerList );
	
protected:
	/** Clean up undo. */
	virtual ~aeUControllerPaste();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Undo action. */
	virtual void Undo();
	
	/** Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif

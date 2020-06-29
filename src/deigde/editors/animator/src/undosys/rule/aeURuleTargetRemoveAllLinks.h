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

#ifndef _AEURULETARGETREMOVEALLLINKS_H_
#define _AEURULETARGETREMOVEALLLINKS_H_

#include "../../animator/link/aeLinkList.h"

#include <deigde/undo/igdeUndo.h>

class aeRule;
class aeControllerTarget;


/**
 * \brief Undo action rule target remove all links.
 */
class aeURuleTargetRemoveAllLinks : public igdeUndo{
private:
	aeRule *pRule;
	aeControllerTarget *pTarget;
	
	aeLinkList pLinks;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	aeURuleTargetRemoveAllLinks( aeRule *rule, aeControllerTarget *target, const aeLinkList &links );
	
protected:
	/** \brief Clean up undo. */
	virtual ~aeURuleTargetRemoveAllLinks();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif

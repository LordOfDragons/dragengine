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
#ifndef _AEURULESASETCONCONTROLLER_H_
#define _AEURULESASETCONCONTROLLER_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class aeRuleSubAnimator;
class aeController;



/**
 * @brief Undo rule sub animator set connection controller.
 *
 * Undo to set controller of a connection of a sub animator rule.
 */
class aeURuleSASetConController : public igdeUndo{
private:
	aeRuleSubAnimator *pRule;
	
	int pTarget;
	aeController *pOldController;
	aeController *pNewController;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	aeURuleSASetConController( aeRuleSubAnimator *rule, int target, aeController *newController );
protected:
	/** \brief Clean up undo. */
	virtual ~aeURuleSASetConController();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif

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

#ifndef _AEURULELIMITSETENABLEPOSYMAX_H_
#define _AEURULELIMITSETENABLEPOSYMAX_H_

#include <deigde/undo/igdeUndo.h>

class aeRuleLimit;



/**
 * Undo action rule limit toggle enable maximum y position.
 */
class aeURuleLimitSetEnablePosYMax : public igdeUndo{
private:
	aeRuleLimit *pRule;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new undo object. */
	aeURuleLimitSetEnablePosYMax( aeRuleLimit *rule );
protected:
	/** Clean up undo. */
	virtual ~aeURuleLimitSetEnablePosYMax();
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

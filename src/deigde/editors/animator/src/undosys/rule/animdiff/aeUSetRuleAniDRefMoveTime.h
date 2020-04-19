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
#ifndef _AEUSETRULEANIDREFMOVETIME_H_
#define _AEUSETRULEANIDREFMOVETIME_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class aeRuleAnimationDifference;



/**
 * @brief Undo set rule animation difference reference move time.
 *
 * Undo to set the reference move time of a animation difference rule.
 */
class aeUSetRuleAniDRefMoveTime : public igdeUndo{
private:
	aeRuleAnimationDifference *pRule;
	
	float pOldTime;
	float pNewTime;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	aeUSetRuleAniDRefMoveTime( aeRuleAnimationDifference *rule, float newTime );
protected:
	/** \brief Clean up undo. */
	virtual ~aeUSetRuleAniDRefMoveTime();
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

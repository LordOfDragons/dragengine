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

#ifndef _AEURULELIMITSETCFRAME_H_
#define _AEURULELIMITSETCFRAME_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/resources/animator/rule/deAnimatorRuleLimit.h>

class aeRuleLimit;



/**
 * Undo action rule limitor set coordinate frame.
 */
class aeURuleLimitSetCFrame : public igdeUndo{
private:
	aeRuleLimit *pRule;
	
	deAnimatorRuleLimit::eCoordinateFrames pOldCoordFrame;
	deAnimatorRuleLimit::eCoordinateFrames pNewCoordFrame;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new undo object. */
	aeURuleLimitSetCFrame( aeRuleLimit *rule, deAnimatorRuleLimit::eCoordinateFrames newCoordFrame );
protected:
	/** Clean up undo. */
	virtual ~aeURuleLimitSetCFrame();
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

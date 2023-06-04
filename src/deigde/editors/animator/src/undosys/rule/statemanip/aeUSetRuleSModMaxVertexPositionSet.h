/* 
 * Drag[en]gine IGDE Animator Editor
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#ifndef _AEUSETRULESMODMAXVERTEXPOSITIONSET_H_
#define _AEUSETRULESMODMAXVERTEXPOSITIONSET_H_

#include <deigde/undo/igdeUndo.h>
#include <dragengine/common/math/decMath.h>

class aeRuleStateManipulator;


/**
 * Undo set rule state modifier maximum vertex position set.
 */
class aeUSetRuleSModMaxVertexPositionSet : public igdeUndo{
private:
	aeRuleStateManipulator *pRule;
	
	float pOldMax;
	float pNewMax;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create undo. */
	aeUSetRuleSModMaxVertexPositionSet( aeRuleStateManipulator *rule, float newMax );
protected:
	/** Clean up undo. */
	virtual ~aeUSetRuleSModMaxVertexPositionSet();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** Undo. */
	virtual void Undo();
	/** Redo. */
	virtual void Redo();
	/*@}*/
	
private:
	void pCleanUp();
};

#endif
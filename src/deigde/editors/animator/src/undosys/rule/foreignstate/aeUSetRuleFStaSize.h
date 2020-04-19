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
#ifndef _AEUSETRULEFSTASIZE_H_
#define _AEUSETRULEFSTASIZE_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class aeRuleForeignState;



/**
 * @brief Undo set rule foreign state size scaling.
 *
 * Undo to set the size scaling of a foreign state rule.
 */
class aeUSetRuleFStaSize : public igdeUndo{
private:
	aeRuleForeignState *pRule;
	
	float pOldScale;
	float pNewScale;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	aeUSetRuleFStaSize( aeRuleForeignState *rule, float newScale );
protected:
	/** \brief Clean up undo. */
	virtual ~aeUSetRuleFStaSize();
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

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

// include only once
#ifndef _MEUHTVRULECOMBINESETY_H_
#define _MEUHTVRULECOMBINESETY_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRuleCombine;



/**
 * \brief Undo Height Terrain Vegetation Rule Combine Set Y.
 *
 * Undo action to set the y component of a height terrain vegetation rule combine.
 */
class meUHTVRuleCombineSetY : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleCombine *pRule;
	
	float pOldY;
	float pNewY;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleCombineSetY( meHTVegetationLayer *vlayer, meHTVRuleCombine *rule, float ny );
	/** \brief Clean up object. */
	virtual ~meUHTVRuleCombineSetY();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif

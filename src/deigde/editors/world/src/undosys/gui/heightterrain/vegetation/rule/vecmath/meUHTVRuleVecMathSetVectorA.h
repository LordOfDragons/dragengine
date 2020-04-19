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
#ifndef _MEUHTVRULEVECMATHVECTORA_H_
#define _MEUHTVRULEVECMATHVECTORA_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRuleVectorMath;



/**
 * \brief Undo Height Terrain Vegetation Rule Vector Math Set Vector A.
 *
 * Undo action to set the A vector of a height terrain vegetation rule vector math.
 */
class meUHTVRuleVecMathSetVectorA : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleVectorMath *pRule;
	
	decVector pOldVector;
	decVector pNewVector;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleVecMathSetVectorA( meHTVegetationLayer *vlayer, meHTVRuleVectorMath *rule, const decVector &nvector );
	/** \brief Clean up object. */
	virtual ~meUHTVRuleVecMathSetVectorA();
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

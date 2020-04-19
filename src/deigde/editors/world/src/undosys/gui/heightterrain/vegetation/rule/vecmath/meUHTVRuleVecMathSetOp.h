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

#ifndef _MEUHTVRULEVECMATHOP_H_
#define _MEUHTVRULEVECMATHOP_H_

#include "../../../../../../world/heightterrain/rules/meHTVRuleVectorMath.h"

#include <deigde/undo/igdeUndo.h>

class meHTVegetationLayer;
class meHTVRuleVectorMath;



/**
 * \brief Undo Height Terrain Vegetation Rule Vector Math Set Operator.
 */
class meUHTVRuleVecMathSetOp : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleVectorMath *pRule;
	
	meHTVRuleVectorMath::eOperators pOldOp;
	meHTVRuleVectorMath::eOperators pNewOp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleVecMathSetOp( meHTVegetationLayer *vlayer, meHTVRuleVectorMath *rule,
		meHTVRuleVectorMath::eOperators nop );
	
protected:
	/** \brief Clean up object. */
	virtual ~meUHTVRuleVecMathSetOp();
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

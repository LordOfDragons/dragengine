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

#ifndef _MEUHTVRULEMULTIMATHOP_H_
#define _MEUHTVRULEMULTIMATHOP_H_

#include "../../../../../../world/heightterrain/rules/meHTVRuleMultiMath.h"

#include <deigde/undo/igdeUndo.h>

class meHTVegetationLayer;
class meHTVRuleMultiMath;



/**
 * \brief Undo Height Terrain Vegetation Rule Multi-Math Set Operator.
 */
class meUHTVRuleMultiMathSetOp : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleMultiMath *pRule;
	
	meHTVRuleMultiMath::eOperators pOldOp;
	meHTVRuleMultiMath::eOperators pNewOp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleMultiMathSetOp( meHTVegetationLayer *vlayer, meHTVRuleMultiMath *rule,
		meHTVRuleMultiMath::eOperators nop );
	
	/** \brief Clean up object. */
	virtual ~meUHTVRuleMultiMathSetOp();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** \brief Undo. */
	virtual void Undo();
	
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

#endif

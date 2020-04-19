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

#ifndef _MEUHTVRULEMATHOP_H_
#define _MEUHTVRULEMATHOP_H_

#include "../../../../../../world/heightterrain/rules/meHTVRuleMath.h"

#include <deigde/undo/igdeUndo.h>

class meHTVegetationLayer;



/**
 * \brief Undo Height Terrain Vegetation Rule Math Set Operator.
 */
class meUHTVRuleMathSetOp : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleMath *pRule;
	
	meHTVRuleMath::eOperators pOldOp;
	meHTVRuleMath::eOperators pNewOp;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleMathSetOp( meHTVegetationLayer *vlayer, meHTVRuleMath *rule, meHTVRuleMath::eOperators nop );
	/** \brief Clean up object. */
	virtual ~meUHTVRuleMathSetOp();
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

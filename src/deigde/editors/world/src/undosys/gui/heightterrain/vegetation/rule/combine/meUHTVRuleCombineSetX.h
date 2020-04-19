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
#ifndef _MEUHTVRULECOMBINESETX_H_
#define _MEUHTVRULECOMBINESETX_H_

// includes
#include <deigde/undo/igdeUndo.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRuleCombine;



/**
 * \brief Undo Height Terrain Vegetation Rule Combine Set X.
 *
 * Undo action to set the x component of a height terrain vegetation rule combine.
 */
class meUHTVRuleCombineSetX : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRuleCombine *pRule;
	
	float pOldX;
	float pNewX;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleCombineSetX( meHTVegetationLayer *vlayer, meHTVRuleCombine *rule, float nx );
	/** \brief Clean up object. */
	virtual ~meUHTVRuleCombineSetX();
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

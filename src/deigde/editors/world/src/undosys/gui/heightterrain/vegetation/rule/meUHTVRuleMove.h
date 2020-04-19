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
#ifndef _MEUHTVRULEMOVE_H_
#define _MEUHTVRULEMOVE_H_

// includes
#include <deigde/undo/igdeUndo.h>

#include <dragengine/common/math/decMath.h>

// predefinitions
class meHTVegetationLayer;
class meHTVRule;



/**
 * \brief Undo Move Height Terrain Vegetation Rule.
 *
 * Undo action for moving a height terrain vegetation rule.
 */
class meUHTVRuleMove : public igdeUndo{
private:
	meHTVegetationLayer *pVLayer;
	meHTVRule *pRule;
	
	decVector2 pOldPosition;
	decVector2 pNewPosition;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create object. */
	meUHTVRuleMove( meHTVegetationLayer *vlayer, meHTVRule *rule );
	/** \brief Clean up object. */
	virtual ~meUHTVRuleMove();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the new position. */
	inline const decVector2 &GetNewPosition() const{ return pNewPosition; }
	/** Sets the new position triggering. */
	void SetNewPosition( const decVector2 &position );
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
};

// end of include only once
#endif

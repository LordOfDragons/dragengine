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
#ifndef _MEHTVRULECLOSESTVEGETATION_H_
#define _MEHTVRULECLOSESTVEGETATION_H_

// includes
#include "meHTVRule.h"

#include <dragengine/common/string/decString.h>

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Closest Vegetation.
 *
 * Determines the distance to the closest vegetation of the given type. The
 * search radius limits the distance to look for vegetations. If no vegetation
 * can be found the search distance is returned. Additionally a boolean value
 * with 0 equal to false and 1 equal to true is returned stating if a vegetation
 * has been found.
 */
class meHTVRuleClosestVegetation : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Distance. */
		eosDistance,
		/** Direction. */
		eosDirection
	};
	
private:
	decString pVegetationType;
	float pSearchRadius;
	
	float pDistance;
	decVector pDirection;
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleClosestVegetation();
	
	/** \brief Create copy of rule. */
	meHTVRuleClosestVegetation( const meHTVRuleClosestVegetation &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleClosestVegetation();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the vegetation type to search for. */
	inline const decString &GetVegetationType() const{ return pVegetationType; }
	/** Sets the vegetation type to search for. */
	void SetVegetationType( const char *vegetationType );
	/** Retrieves the search radius. */
	inline float GetSearchRadius() const{ return pSearchRadius; }
	/** Sets the search radius. */
	void SetSearchRadius( float searchRadius );
	
	/** Update the result if required. */
	void UpdateResult( meHTVEvaluationEnvironment &evalEnv );
	
	/** Resets the rule state. */
	virtual void Reset();
	/** Retrieves the value of a given output slot. */
	virtual float GetOutputSlotValueAt( int slot, meHTVEvaluationEnvironment &evalEnv );
	/** Retrieves the vector of a given output slot. */
	virtual decVector GetOutputSlotVectorAt( int slot, meHTVEvaluationEnvironment &evalEnv );
	
	/** \brief Copy rule. */
	virtual meHTVRule *Copy() const;
	/*@}*/
};

// end of include only once
#endif

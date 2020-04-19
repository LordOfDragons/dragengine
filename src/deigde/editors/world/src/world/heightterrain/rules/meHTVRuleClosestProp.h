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
#ifndef _MEHTVRULECLOSESTPROP_H_
#define _MEHTVRULECLOSESTPROP_H_

// includes
#include "meHTVRule.h"

#include <dragengine/common/string/decString.h>

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Closest Prop.
 *
 * Determines the distance to the closest prop of a given class. The search
 * radius limits the distance to look for props. If no prop can be found the
 * search distance is returned. Additionally a boolean value with 0 equal to
 * false and 1 equal to true is returned stating if a prop has been found.
 * The prop is looked for in both height terrain vegetation layers as well
 * as objects in the world.
 */
class meHTVRuleClosestProp : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Distance. */
		eosDistance,
		/** Direction. */
		eosDirection
	};
	
private:
	decString pPropClass;
	float pSearchRadius;
	
	float pDistance;
	decVector pDirection;
	bool pDirty;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleClosestProp();
	
	/** \brief Create copy of rule. */
	meHTVRuleClosestProp( const meHTVRuleClosestProp &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleClosestProp();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the class name of the prop to search for. */
	inline const decString &GetPropClass() const{ return pPropClass; }
	/** Sets the class name of the prop to search for. */
	void SetPropClass( const char *propClass );
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

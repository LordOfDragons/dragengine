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
#ifndef _MEHTVRULERANDOM_H_
#define _MEHTVRULERANDOM_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Random.
 *
 * Source type rule providing random values.
 */
class meHTVRuleRandom : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Random value. */
		eosRandom
	};
	
private:
	float pRandom;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleRandom();
	
	/** \brief Create copy of rule. */
	meHTVRuleRandom( const meHTVRuleRandom &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleRandom();
	/*@}*/
	
	/** @name Management */
	/*@{*/
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

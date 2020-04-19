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
#ifndef _MEHTVRULEMAPPING_H_
#define _MEHTVRULEMAPPING_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Mapping.
 *
 * Maps an input value into a range A to B where A and B can be altered
 * using inputs themselves or stay constant. Output is the value clamped
 * to the range from A to B and mapped to 0 to 1 where A maps to 0 and
 * B maps to 1 with values inbetween mapped linearly.
 */
class meHTVRuleMapping : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Mapped value. */
		eosValue,
		/** Lower bound. */
		eisLower,
		/** Upper bound. */
		eisUpper,
		/** Value. */
		eisValue
	};
	
private:
	float pValue;
	float pLower;
	float pUpper;
	bool pInversed;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleMapping();
	
	/** \brief Create copy of rule. */
	meHTVRuleMapping( const meHTVRuleMapping &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleMapping();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the value to use if there is no input value. */
	inline float GetValue() const{ return pValue; }
	/** Sets the value to use if there is no input value. */
	void SetValue( float value );
	/** Retrieves the lower bound to use if there is no input lower bound. */
	inline float GetLower() const{ return pLower; }
	/** Sets the lower bound to use if there is no input lower bound. */
	void SetLower( float lower );
	/** Retrieves the upper bound to use if there is no input upper bound. */
	inline float GetUpper() const{ return pUpper; }
	/** Sets the upper bound to use if there is no input upper bound. */
	void SetUpper( float upper );
	/** Determines if the mapping is inversed. */
	inline bool GetInversed() const{ return pInversed; }
	/** Sets if the mapping is inverse. */
	void SetInversed( bool inversed );
	
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

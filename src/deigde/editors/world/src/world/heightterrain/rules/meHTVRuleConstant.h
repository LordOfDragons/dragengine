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
#ifndef _MEHTVRULECONSTANT_H_
#define _MEHTVRULECONSTANT_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Constant.
 *
 * Provides a constant vector as input for other rules. The components
 * of the constant vector can also be read individually allowing one
 * constant rule to be used to input 3 different values.
 */
class meHTVRuleConstant : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Vector. */
		eosVector,
		/** X Component. */
		eosX,
		/** Y Component. */
		eosY,
		/** Z Component. */
		eosZ
	};
	
private:
	decVector pVector;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleConstant();
	
	/** \brief Create copy of rule. */
	meHTVRuleConstant( const meHTVRuleConstant &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleConstant();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the vector to output. */
	inline const decVector &GetVector() const{ return pVector; }
	/** Sets the vector to output. */
	void SetVector( const decVector &vector );
	
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

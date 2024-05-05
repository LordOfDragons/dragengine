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
#ifndef _MEHTVRULEMULTIMATH_H_
#define _MEHTVRULEMULTIMATH_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Multi Math.
 *
 * Applies a math operator to the list of input values. This rule is similar
 * to the math rule but supports only the four operators which can operate
 * on a list of values and are commutative. Namely these are addition,
 * multiplcation, minimum value and maximum value.
 */
class meHTVRuleMultiMath : public meHTVRule{
public:
	/** Operators. */
	enum eOperators{
		/** Addition. */
		eopAdd,
		/** Multiplication. */
		eopMultiply,
		/** Minimum Value. */
		eopMinimum,
		/** Maximum Value. */
		eopMaximum,
		/** Average between all values. */
		eopAverage,
	};
	
	/** \brief Slots. */
	enum eSlots{
		/** Result. */
		eosResult,
		/** Values. */
		eisValues,
	};
	
private:
	eOperators pOperator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleMultiMath();
	
	/** \brief Create copy of rule. */
	meHTVRuleMultiMath( const meHTVRuleMultiMath &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleMultiMath();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	/** Sets the operator. */
	void SetOperator( eOperators oper );
	
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

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
#ifndef _MEHTVRULEVECTORMATH_H_
#define _MEHTVRULEVECTORMATH_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Vector Math.
 *
 * Applies a binary or unary vector math operator to the input vectors.
 * The first input is named A and the second B for the remainder of this
 * comment. In the case of undefined results a 0 vector is returned.
 * Angles are measured in degrees where applicable. The return value is
 * either a value or a vector depending on the operator. So use the
 * right output slot to fetch the result.
 */
class meHTVRuleVectorMath : public meHTVRule{
public:
	/** Operators. */
	enum eOperators{
		/** A + B, component wise. */
		eopAdd,
		/** A - B, component wise. */
		eopSubtract,
		/** Average between A and B component wise. */
		eopAverage,
		/** Normalize. */
		eopNormalize,
		/** Dot product. */
		eopDot,
		/** Cross product. */
		eopCross
	};
	
	/** \brief Slots. */
	enum eSlots{
		/** Value result. */
		eosValue,
		/** Vector result. */
		eosVector,
		/** Value A. */
		eisVectorA,
		/** Value B. */
		eisVectorB
	};
	
private:
	decVector pVectorA;
	decVector pVectorB;
	eOperators pOperator;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleVectorMath();
	
	/** \brief Create copy of rule. */
	meHTVRuleVectorMath( const meHTVRuleVectorMath &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleVectorMath();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the vector-A to use if there is no input vector-A. */
	inline const decVector &GetVectorA() const{ return pVectorA; }
	/** Sets the vector-A to use if there is no input vector-A. */
	void SetVectorA( const decVector &value );
	/** Retrieves the vector-B to use if there is no input vector-B. */
	inline const decVector &GetVectorB() const{ return pVectorB; }
	/** Sets the vector-B to use if there is no input vector-B. */
	void SetVectorB( const decVector &value );
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

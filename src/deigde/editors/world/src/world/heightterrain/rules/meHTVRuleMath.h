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
#ifndef _MEHTVRULEMATH_H_
#define _MEHTVRULEMATH_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Math.
 *
 * Applies a binary or unary math operator to the input values. The
 * first input is named A and the second B for the remainder of this
 * comment. In the case of undefined results 0 is returned. All
 * trigonometric operators use degrees as unit of angles. Boolean
 * operators return 0 for false and 1 for true.
 */
class meHTVRuleMath : public meHTVRule{
public:
	/** Operators. */
	enum eOperators{
		/** A + B. */
		eopAdd,
		/** A - B. */
		eopSubtract,
		/** A * B. */
		eopMultiply,
		/** A / B. */
		eopDivide,
		/** sin( A ). */
		eopSine,
		/** cos( A ). */
		eopCosine,
		/** tan( A ). */
		eopTangent,
		/** arcsin( A ). */
		eopArcSine,
		/** arccos( A ). */
		eopArcCosine,
		/** arctan( A ). */
		eopArcTangent,
		/** pow( A, B ). */
		eopPower,
		/** exp( A ) = pow( A, e ). */
		eopExponential,
		/** log( A ) = log-base-e( A ). */
		eopLogarithm,
		/** min( A, B ). */
		eopMinimum,
		/** max( A, B ). */
		eopMaximum,
		/** round A to B. */
		eopRound,
		/** A &lt; B. */
		eopLessThan,
		/** A &gt; B. */
		eopGreaterThan,
		/** A is equal to B inside a small epsilon gap. */
		eopEqual,
		/** A is not equal to B in side a small epsilon gap. */
		eopNotEqual,
		/** Average between A and B. */
		eopAverage,
	};
	
	/** \brief Slots. */
	enum eSlots{
		/** Result. */
		eosResult,
		/** Value A. */
		eisValueA,
		/** Value B. */
		eisValueB
	};
	
private:
	float pValueA;
	float pValueB;
	eOperators pOperator;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleMath();
	
	/** \brief Create copy of rule. */
	meHTVRuleMath( const meHTVRuleMath &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleMath();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the value-A to use if there is no input value-A. */
	inline float GetValueA() const{ return pValueA; }
	/** Sets the value-A to use if there is no input value-A. */
	void SetValueA( float value );
	/** Retrieves the value-B to use if there is no input value-B. */
	inline float GetValueB() const{ return pValueB; }
	/** Sets the value-B to use if there is no input value-B. */
	void SetValueB( float value );
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

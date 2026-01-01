/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleMath> Ref;
	
	
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
	meHTVRuleMath(const meHTVRuleMath &rule);
	
protected:
	/** Cleans up the rule. */
	~meHTVRuleMath() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the value-A to use if there is no input value-A. */
	inline float GetValueA() const{ return pValueA; }
	/** Sets the value-A to use if there is no input value-A. */
	void SetValueA(float value);
	/** Retrieves the value-B to use if there is no input value-B. */
	inline float GetValueB() const{ return pValueB; }
	/** Sets the value-B to use if there is no input value-B. */
	void SetValueB(float value);
	/** Retrieves the operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	/** Sets the operator. */
	void SetOperator(eOperators oper);
	
	/** Retrieves the value of a given output slot. */
	float GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv) override;
	/** Retrieves the vector of a given output slot. */
	virtual decVector GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv);
	
	/** \brief Copy rule. */
	meHTVRule::Ref Copy() const override;
	/*@}*/
};

// end of include only once
#endif

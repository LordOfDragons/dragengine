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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleMultiMath> Ref;
	
	
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
	meHTVRuleMultiMath(const meHTVRuleMultiMath &rule);
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleMultiMath();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the operator. */
	inline eOperators GetOperator() const{ return pOperator; }
	/** Sets the operator. */
	void SetOperator(eOperators oper);
	
	/** Retrieves the value of a given output slot. */
	virtual float GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv);
	/** Retrieves the vector of a given output slot. */
	virtual decVector GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv);
	
	/** \brief Copy rule. */
	virtual meHTVRule *Copy() const;
	/*@}*/
};

// end of include only once
#endif

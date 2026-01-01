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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleVectorMath> Ref;
	
	
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleVectorMath();
	
	/** \brief Create copy of rule. */
	meHTVRuleVectorMath(const meHTVRuleVectorMath &rule);
	
protected:
	/** Cleans up the rule. */
	~meHTVRuleVectorMath() override;
	
public:
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the vector-A to use if there is no input vector-A. */
	inline const decVector &GetVectorA() const{ return pVectorA; }
	/** Sets the vector-A to use if there is no input vector-A. */
	void SetVectorA(const decVector &value);
	/** Retrieves the vector-B to use if there is no input vector-B. */
	inline const decVector &GetVectorB() const{ return pVectorB; }
	/** Sets the vector-B to use if there is no input vector-B. */
	void SetVectorB(const decVector &value);
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

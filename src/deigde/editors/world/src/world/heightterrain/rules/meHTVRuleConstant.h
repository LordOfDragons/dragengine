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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleConstant> Ref;
	
	
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleConstant();
	
	/** \brief Create copy of rule. */
	meHTVRuleConstant(const meHTVRuleConstant &rule);
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleConstant();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the vector to output. */
	inline const decVector &GetVector() const{return pVector;}
	/** Sets the vector to output. */
	void SetVector(const decVector &vector);
	
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

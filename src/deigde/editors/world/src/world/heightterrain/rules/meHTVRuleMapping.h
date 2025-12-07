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
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleMapping> Ref;
	
	
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleMapping();
	
	/** \brief Create copy of rule. */
	meHTVRuleMapping(const meHTVRuleMapping &rule);
	
	/** Cleans up the rule. */
	~meHTVRuleMapping() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the value to use if there is no input value. */
	inline float GetValue() const{ return pValue; }
	/** Sets the value to use if there is no input value. */
	void SetValue(float value);
	/** Retrieves the lower bound to use if there is no input lower bound. */
	inline float GetLower() const{ return pLower; }
	/** Sets the lower bound to use if there is no input lower bound. */
	void SetLower(float lower);
	/** Retrieves the upper bound to use if there is no input upper bound. */
	inline float GetUpper() const{ return pUpper; }
	/** Sets the upper bound to use if there is no input upper bound. */
	void SetUpper(float upper);
	/** Determines if the mapping is inversed. */
	inline bool GetInversed() const{ return pInversed; }
	/** Sets if the mapping is inverse. */
	void SetInversed(bool inversed);
	
	/** Retrieves the value of a given output slot. */
	float GetOutputSlotValueAt(int slot, meHTVEvaluationEnvironment &evalEnv) override;
	/** Retrieves the vector of a given output slot. */
	virtual decVector GetOutputSlotVectorAt(int slot, meHTVEvaluationEnvironment &evalEnv);
	
	/** \brief Copy rule. */
	meHTVRule *Copy() const override;
	/*@}*/
};

// end of include only once
#endif

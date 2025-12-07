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
#ifndef _MEHTVRULECOMPONENTS_H_
#define _MEHTVRULECOMPONENTS_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Components.
 *
 * Takes a vector as sole input and provides the 3 components as output.
 */
class meHTVRuleComponents : public meHTVRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleComponents> Ref;
	
	
	/** \brief Slots. */
	enum eSlots{
		/** X Component. */
		eosX,
		/** Y Component. */
		eosY,
		/** Z Component. */
		eosZ,
		/** Vector. */
		eisVector
	};
	
private:
	decVector pVector;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleComponents();
	
	/** \brief Create copy of rule. */
	meHTVRuleComponents(const meHTVRuleComponents &rule);
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleComponents();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the vector to use if there is no input vector. */
	inline const decVector &GetVector() const{ return pVector; }
	/** Sets the vector to use if there is no input vector. */
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

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
#ifndef _MEHTVRULECOMBINE_H_
#define _MEHTVRULECOMBINE_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Combine.
 *
 * Takes 3 values as input producing a vector using the input values as
 * X, Y and Z components.
 */
class meHTVRuleCombine : public meHTVRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleCombine> Ref;
	
	
	/** \brief Slots. */
	enum eSlots{
		/** Vector. */
		eosVector,
		/** X Component. */
		eisX,
		/** Y Component. */
		eisY,
		/** Z Component. */
		eisZ
	};
	
private:
	float pX;
	float pY;
	float pZ;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleCombine();
	
	/** \brief Create copy of rule. */
	meHTVRuleCombine(const meHTVRuleCombine &rule);
	
	/** Cleans up the rule. */
	~meHTVRuleCombine() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the x value to use if there is no x input. */
	inline float GetX() const{ return pX; }
	/** Sets the x value to use if there is no x input. */
	void SetX(float x);
	/** Retrieves the y value to use if there is no y input. */
	inline float GetY() const{ return pY; }
	/** Sets the y value to use if there is no y input. */
	void SetY(float y);
	/** Retrieves the z value to use if there is no z input. */
	inline float GetZ() const{ return pZ; }
	/** Sets the z value to use if there is no z input. */
	void SetZ(float z);
	
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

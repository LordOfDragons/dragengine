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
#ifndef _MEHTVRULECLOSESTPROP_H_
#define _MEHTVRULECLOSESTPROP_H_

// includes
#include "meHTVRule.h"

#include <dragengine/common/string/decString.h>

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Closest Prop.
 *
 * Determines the distance to the closest prop of a given class. The search
 * radius limits the distance to look for props. If no prop can be found the
 * search distance is returned. Additionally a boolean value with 0 equal to
 * false and 1 equal to true is returned stating if a prop has been found.
 * The prop is looked for in both height terrain vegetation layers as well
 * as objects in the world.
 */
class meHTVRuleClosestProp : public meHTVRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRuleClosestProp> Ref;
	
	
	/** \brief Slots. */
	enum eSlots{
		/** Distance. */
		eosDistance,
		/** Direction. */
		eosDirection
	};
	
private:
	decString pPropClass;
	float pSearchRadius;
	
	float pDistance;
	decVector pDirection;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleClosestProp();
	
	/** \brief Create copy of rule. */
	meHTVRuleClosestProp(const meHTVRuleClosestProp &rule);
	
	/** Cleans up the rule. */
	~meHTVRuleClosestProp() override;
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the class name of the prop to search for. */
	inline const decString &GetPropClass() const{ return pPropClass; }
	/** Sets the class name of the prop to search for. */
	void SetPropClass(const char *propClass);
	/** Retrieves the search radius. */
	inline float GetSearchRadius() const{ return pSearchRadius; }
	/** Sets the search radius. */
	void SetSearchRadius(float searchRadius);
	
	/** Update the result if required. */
	void UpdateResult(meHTVEvaluationEnvironment &evalEnv);
	
	/** Resets the rule state. */
	void Reset() override;
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

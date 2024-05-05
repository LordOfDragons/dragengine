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
#ifndef _MEHTVRULECLOSESTVEGETATION_H_
#define _MEHTVRULECLOSESTVEGETATION_H_

// includes
#include "meHTVRule.h"

#include <dragengine/common/string/decString.h>

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Closest Vegetation.
 *
 * Determines the distance to the closest vegetation of the given type. The
 * search radius limits the distance to look for vegetations. If no vegetation
 * can be found the search distance is returned. Additionally a boolean value
 * with 0 equal to false and 1 equal to true is returned stating if a vegetation
 * has been found.
 */
class meHTVRuleClosestVegetation : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Distance. */
		eosDistance,
		/** Direction. */
		eosDirection
	};
	
private:
	decString pVegetationType;
	float pSearchRadius;
	
	float pDistance;
	decVector pDirection;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleClosestVegetation();
	
	/** \brief Create copy of rule. */
	meHTVRuleClosestVegetation( const meHTVRuleClosestVegetation &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleClosestVegetation();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the vegetation type to search for. */
	inline const decString &GetVegetationType() const{ return pVegetationType; }
	/** Sets the vegetation type to search for. */
	void SetVegetationType( const char *vegetationType );
	/** Retrieves the search radius. */
	inline float GetSearchRadius() const{ return pSearchRadius; }
	/** Sets the search radius. */
	void SetSearchRadius( float searchRadius );
	
	/** Update the result if required. */
	void UpdateResult( meHTVEvaluationEnvironment &evalEnv );
	
	/** Resets the rule state. */
	virtual void Reset();
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

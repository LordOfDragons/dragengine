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
#ifndef _MEHTVRULEPROPCOUNT_H_
#define _MEHTVRULEPROPCOUNT_H_

// includes
#include "meHTVRule.h"

#include <dragengine/common/string/decString.h>

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Prop Count.
 *
 * Determines the number of props of a given class in a given radius around
 * the instance.
 */
class meHTVRulePropCount : public meHTVRule{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<meHTVRulePropCount> Ref;
	
	
	/** \brief Slots. */
	enum eSlots{
		/** Count. */
		eosCount
	};
	
private:
	decString pPropClass;
	float pSearchRadius;
	
	float pCount;
	bool pDirty;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRulePropCount();
	
	/** \brief Create copy of rule. */
	meHTVRulePropCount( const meHTVRulePropCount &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRulePropCount();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the class name of the prop to search for. */
	inline const decString &GetPropClass() const{ return pPropClass; }
	/** Sets the class name of the prop to search for. */
	void SetPropClass( const char *propClass );
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

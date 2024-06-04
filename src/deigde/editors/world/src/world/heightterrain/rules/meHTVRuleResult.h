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
#ifndef _MEHTVRULERESULT_H_
#define _MEHTVRULERESULT_H_

// includes
#include "meHTVRule.h"

// predefinitions



/**
 * @brief Height Terrain Vegetation Rule Result.
 *
 * Stores the input of the "probability" input slot into the
 * evaluation environment as the result of the evaluation.
 */
class meHTVRuleResult : public meHTVRule{
public:
	/** \brief Slots. */
	enum eSlots{
		/** Probability. */
		eisProbability,
		/** Variation. */
		eisVariation
	};
	
private:
	float pProbability;
	int pVariation;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleResult();
	
	/** \brief Create copy of rule. */
	meHTVRuleResult( const meHTVRuleResult &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleResult();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Probability to use if there is no input probability. */
	inline float GetProbability() const{ return pProbability; }
	/** Sets the probability to use if there is no input probability. */
	void SetProbability( float probability );
	/** Variation to use if there is no input variation. */
	inline int GetVariation() const{ return pVariation; }
	/** Sets the variation to use if there is no input variation. */
	void SetVariation( int variation );
	
	/** Evaluate rule. */
	virtual void Evaluate( meHTVEvaluationEnvironment &evalEnv );
	
	/** \brief Copy rule. */
	virtual meHTVRule *Copy() const;
	/*@}*/
};

// end of include only once
#endif

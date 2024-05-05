/* 
 * Drag[en]gine IGDE World Editor
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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

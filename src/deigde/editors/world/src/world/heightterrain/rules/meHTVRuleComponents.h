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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleComponents();
	
	/** \brief Create copy of rule. */
	meHTVRuleComponents( const meHTVRuleComponents &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleComponents();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the vector to use if there is no input vector. */
	inline const decVector &GetVector() const{ return pVector; }
	/** Sets the vector to use if there is no input vector. */
	void SetVector( const decVector &vector );
	
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

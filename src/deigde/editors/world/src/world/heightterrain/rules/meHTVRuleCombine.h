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
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new rule. */
	meHTVRuleCombine();
	
	/** \brief Create copy of rule. */
	meHTVRuleCombine( const meHTVRuleCombine &rule );
	
	/** Cleans up the rule. */
	virtual ~meHTVRuleCombine();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the x value to use if there is no x input. */
	inline float GetX() const{ return pX; }
	/** Sets the x value to use if there is no x input. */
	void SetX( float x );
	/** Retrieves the y value to use if there is no y input. */
	inline float GetY() const{ return pY; }
	/** Sets the y value to use if there is no y input. */
	void SetY( float y );
	/** Retrieves the z value to use if there is no z input. */
	inline float GetZ() const{ return pZ; }
	/** Sets the z value to use if there is no z input. */
	void SetZ( float z );
	
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

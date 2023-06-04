/* 
 * Drag[en]gine Animator Module
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

#ifndef _DEARCONTROLLERSTATES_H_
#define _DEARCONTROLLERSTATES_H_

#include <dragengine/common/math/decMath.h>



/**
 * Controller States.
 */
class dearControllerStates{
private:
	struct sState{
		float value;
		decVector vector;
	};
	
private:
	sState *pStates;
	int pStateCount;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Creates a new controller states object. */
	dearControllerStates();
	/** Cleans up the controller states object. */
	~dearControllerStates();
	/*@}*/
	
	/** \name Management */
	/*@{*/
	/** Retrieves the number of states. */
	inline int GetStateCount() const{ return pStateCount; }
	/** Sets the number of states. */
	void SetStateCount( int count );
	
	/** Retrieves the value of a state. */
	float GetValueAt( int state ) const;
	/** Retrieves the vector of a state. */
	const decVector &GetVectorAt( int state ) const;
	/** Sets the value of a state. */
	void SetValueAt( int state, float value );
	/** Sets the vector of a state. */
	void SetVectorAt( int state, const decVector &vector );
	/** Sets the state. */
	void SetStateAt( int state, float value, const decVector &vector );
	/** Resets a state. */
	void ResetStateAt( int state );
	/** Set state from another controller states. */
	void SetStateFrom( int destState, const dearControllerStates &sourceStates, int sourceState );
	/*@}*/
};

#endif

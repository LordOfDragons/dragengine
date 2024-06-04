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

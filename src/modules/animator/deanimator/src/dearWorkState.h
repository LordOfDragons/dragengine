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

#ifndef _DEARWORKSTATE_H_
#define _DEARWORKSTATE_H_

#include <dragengine/common/math/decMath.h>

class dearBoneState;



/**
 * @brief Work state class.
 * Stores the working state of a bone for rules doing complex calculations on bones before they are
 * applied to the bone states. Stores only the global matrix as this one required during complex
 * calculations. Stores also a link to the bone state it is applied to in the end.
 */
class dearWorkState{
private:
	dearBoneState *pBoneState;
	decMatrix pGlobalMatrix;
	decMatrix pInvGlobalMatrix;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new bone state object. */
	dearWorkState();
	/** Cleans up the bone state. */
	~dearWorkState();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the linked bone state. */
	inline dearBoneState *GetBoneState() const{ return pBoneState; }
	/** Sets the linked bone state. */
	void SetBoneState( dearBoneState *boneState );
	/** Retrieves the global matrix. */
	inline const decMatrix &GetGlobalMatrix() const{ return pGlobalMatrix; }
	/** Sets the global matrix. */
	void SetGlobalMatrix( const decMatrix &matrix );
	/** Retrieves the inverse global matrix. */
	inline const decMatrix &GetInverseGlobalMatrix() const{ return pInvGlobalMatrix; }
	/** Sets the inverse global matrix. */
	void SetInverseGlobalMatrix( const decMatrix &matrix );
	/*@}*/
};

#endif

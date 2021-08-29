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

#ifndef _DEARIKWORKSTATE_H_
#define _DEARIKWORKSTATE_H_

#include <dragengine/common/math/decMath.h>

class dearBoneState;



/**
 * Working state of bone for inverse kinematic solving.
 */
class dearIKWorkState{
public:
	/** Axis Types. */
	enum eAxisTypes{
		/** Locked. */
		eatLocked,
		
		/** Limited. */
		eatLimited,
		
		/** Free. */
		eatFree
	};
	
	
	
private:
	int pBoneStateIndex;
	decMatrix pGlobalMatrix;
	decMatrix pInvGlobalMatrix;
	decVector pRefRotation;
	int pAxisTypeX;
	int pAxisTypeY;
	int pAxisTypeZ;
	bool pHasLimits;
	decVector pDampening;
	bool pHasDampening;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create bone state. */
	dearIKWorkState();
	
	/** Clean up bone state. */
	~dearIKWorkState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Bone state index. */
	inline int GetBoneStateIndex() const{ return pBoneStateIndex; }
	
	/** Set bone state index. */
	void SetBoneStateIndex( int index );
	
	/** Global matrix. */
	inline const decMatrix &GetGlobalMatrix() const{ return pGlobalMatrix; }
	
	/** Set global matrix. */
	void SetGlobalMatrix( const decMatrix &matrix );
	
	/** Inverse global matrix. */
	inline const decMatrix &GetInverseGlobalMatrix() const{ return pInvGlobalMatrix; }
	
	/** Set inverse global matrix. */
	void SetInverseGlobalMatrix( const decMatrix &matrix );
	
	
	
	/** Reference rotation. */
	inline const decVector &GetReferenceRotation() const{ return pRefRotation; }
	
	/** Set reference rotation. */
	void SetReferenceRotation( const decVector &rotation );
	
	/** X axis type. */
	inline int GetAxisTypeX() const{ return pAxisTypeX; }
	
	/** Set X axis type. */
	void SetAxisTypeX( int type );
	
	/** Y axis type. */
	inline int GetAxisTypeY() const{ return pAxisTypeY; }
	
	/** Set Y axis type. */
	void SetAxisTypeY( int type );
	
	/** Z axis type. */
	inline int GetAxisTypeZ() const{ return pAxisTypeZ; }
	
	/** Set Z axis type. */
	void SetAxisTypeZ( int type );
	
	/** Any limits in effect. */
	inline bool GetHasLimits() const{ return pHasLimits; }
	
	/** Set if any limits are in effect. */
	void SetHasLimits( bool hasLimits );
	
	/** Dampening. */
	inline const decVector &GetDampening() const{ return pDampening; }
	
	/** Set dampening. */
	void SetDampening( const decVector &resistance );
	
	/** Has dampening. */
	inline bool HasDampening() const{ return pHasDampening; }
	/*@}*/
};

#endif

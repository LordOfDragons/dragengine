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
 * \brief Working state of bone for inverse kinematic solving.
 */
class dearIKWorkState{
public:
	/** \brief Axis Types. */
	enum eAxisTypes{
		/** \brief Locked. */
		eatLocked,
		
		/** \brief Limited. */
		eatLimited,
		
		/** \brief Free. */
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
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bone state. */
	dearIKWorkState();
	
	/** \brief Clean up bone state. */
	~dearIKWorkState();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bone state index. */
	inline int GetBoneStateIndex() const{ return pBoneStateIndex; }
	
	/** \brief Set bone state index. */
	void SetBoneStateIndex( int index );
	
	/** \brief Global matrix. */
	inline const decMatrix &GetGlobalMatrix() const{ return pGlobalMatrix; }
	
	/** \brief Set global matrix. */
	void SetGlobalMatrix( const decMatrix &matrix );
	
	/** \brief Inverse global matrix. */
	inline const decMatrix &GetInverseGlobalMatrix() const{ return pInvGlobalMatrix; }
	
	/** \brief Set inverse global matrix. */
	void SetInverseGlobalMatrix( const decMatrix &matrix );
	
	
	
	/** \brief Reference rotation. */
	inline const decVector &GetReferenceRotation() const{ return pRefRotation; }
	
	/** \brief Set reference rotation. */
	void SetReferenceRotation( const decVector &rotation );
	
	/** \brief X axis type. */
	inline int GetAxisTypeX() const{ return pAxisTypeX; }
	
	/** \brief Set X axis type. */
	void SetAxisTypeX( int type );
	
	/** \brief Y axis type. */
	inline int GetAxisTypeY() const{ return pAxisTypeY; }
	
	/** \brief Set Y axis type. */
	void SetAxisTypeY( int type );
	
	/** \brief Z axis type. */
	inline int GetAxisTypeZ() const{ return pAxisTypeZ; }
	
	/** \brief Set Z axis type. */
	void SetAxisTypeZ( int type );
	
	/** \brief Any limits in effect. */
	inline bool GetHasLimits() const{ return pHasLimits; }
	
	/** \brief Set if any limits are in effect. */
	void SetHasLimits( bool hasLimits );
	/*@}*/
};

#endif

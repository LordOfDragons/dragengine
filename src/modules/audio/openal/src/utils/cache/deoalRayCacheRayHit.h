/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALRAYCACHERAYHIT_H_
#define _DEOALRAYCACHERAYHIT_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Model ray cache ray hit.
 */
class deoalRayCacheRayHit{
	float pDistance;
	int pFaceIndex;
	bool pForwardFacing;
	decVector pPoint;
	decVector pNormal;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create model ray cache ray hit. */
	deoalRayCacheRayHit();
	
	/** \brief Clean up model ray cache ray hit. */
	~deoalRayCacheRayHit();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Distance along ray to hit point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance along ray to hit point. */
	void SetDistance( float distance );
	
	/** \brief Index of hit face. */
	inline int GetFaceIndex() const{ return pFaceIndex; }
	
	/** \brief Set index of hit face. */
	void SetFaceIndex( int faceIndex );
	
	/** \brief Forward facing. */
	inline bool GetForwardFacing() const{ return pForwardFacing; }
	
	/** \brief Set forward facing. */
	void SetForwardFacing( bool forwardFacing );
	
	/** \brief Hit point. */
	inline const decVector &GetPoint() const{ return pPoint; }
	
	/** \brief Set hit point. */
	void SetPoint( const decVector &point );
	
	/** \brief Normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set normal. */
	void SetNormal( const decVector &normal );
	/*@}*/
};

#endif

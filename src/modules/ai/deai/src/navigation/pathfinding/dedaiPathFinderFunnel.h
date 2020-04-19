/* 
 * Drag[en]gine AI Module
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

#ifndef _DEDAIPATHFINDERFUNNEL_H_
#define _DEDAIPATHFINDERFUNNEL_H_

#include <dragengine/common/math/decMath.h>

class dedaiSpaceMesh;



/**
 * @brief Path Finder funnel.
 */
class dedaiPathFinderFunnel{
private:
	dedaiSpaceMesh *pMesh;
	decVector pOrigin;
	decVector pGoal;
	decVector pLeftCorner;
	decVector pRightCorner;
	decVector pLeftNormal;
	decVector pRightNormal;
	float pLeftDist;
	float pRightDist;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new path finder funnel. */
	dedaiPathFinderFunnel();
	/** Cleans up the path finder funnel. */
	~dedaiPathFinderFunnel();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the navigation mesh the funnel data is relative to. */
	inline dedaiSpaceMesh *GetMesh() const{ return pMesh; }
	/** Sets the navigation mesh the funnel data is relative to. */
	void SetMesh( dedaiSpaceMesh *mesh );
	/** Retrieves the origin. */
	inline const decVector &GetOrigin() const{ return pOrigin; }
	/** Sets the origin. */
	void SetOrigin( const decVector &origin );
	/** Retrieves the goal. */
	inline const decVector &GetGoal() const{ return pGoal; }
	/** Sets the goal. */
	void SetGoal( const decVector &goal );
	
	/** Retrieves the left corner. */
	inline const decVector &GetLeftCorner() const{ return pLeftCorner; }
	/** Sets the left corner. */
	void SetLeftCorner( const decVector &corner );
	/** Retrieves the left plane normal. */
	inline const decVector &GetLeftNormal() const{ return pLeftNormal; }
	/** Sets the left plane normal. */
	void SetLeftNormal( const decVector &normal );
	/** Retrieves the left plane distance. */
	inline float GetLeftDistance() const{ return pLeftDist; }
	
	/** Retrieves the right corner. */
	inline const decVector &GetRightCorner() const{ return pRightCorner; }
	/** Sets the right corner. */
	void SetRightCorner( const decVector &corner );
	/** Retrieves the right plane normal. */
	inline const decVector &GetRightNormal() const{ return pRightNormal; }
	/** Sets the right plane normal. */
	void SetRightNormal( const decVector &normal );
	/** Retrieves the right plane distance. */
	inline float GetRightDistance() const{ return pRightDist; }	
	
	/** Update left plane using the current origin and left normal. */
	void UpdateLeftPlane( const decVector &faceNormal );
	/** Update right plane using the current origin and right normal. */
	void UpdateRightPlane( const decVector &faceNormal );
	/** Transform funnel by a matrix. */
	void Transform( const decMatrix &matrix );
	/*@}*/
};

#endif

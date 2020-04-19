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

#ifndef _DEOALRAYTRACEHITELEMENT_H_
#define _DEOALRAYTRACEHITELEMENT_H_

#include <dragengine/common/math/decMath.h>

class deoalAComponent;


/**
 * \brief Ray tracing hit element.
 */
class deoalRayTraceHitElement{
private:
	float pDistance;
	decDVector pPoint;
	decDVector pNormal;
	bool pForwardFacing;
	
	deoalAComponent *pComponent;
	int pComponentFace;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty ray trace hit element. */
	deoalRayTraceHitElement();
	
	/** \brief Create copy of ray trace hit element. */
	deoalRayTraceHitElement( const deoalRayTraceHitElement &element );
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Distance in meters to hit point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Hit point in world coordinates. */
	inline const decDVector &GetPoint() const{ return pPoint; }
	
	/** \brief Hit normal in world coordinates. */
	inline const decDVector &GetNormal() const{ return pNormal; }
	
	/** \brief Hit element is forward facing. */
	inline bool GetForwardFacing() const{ return pForwardFacing; }
	
	
	
	/** \brief Hit component or NULL. */
	inline deoalAComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Hit component face or -1. */
	inline int GetComponentFace() const{ return pComponentFace; }
	
	
	
	/** \brief Set empty. */
	void SetEmpty();
	
	/** \brief Set element to hit component face. */
	void SetComponentFace( float distance, const decDVector &point, const decDVector &normal,
		deoalAComponent *component, int face, bool frontFacing );
	
	
	
	/** \brief Assign element. */
	deoalRayTraceHitElement &operator=( const deoalRayTraceHitElement &element );
	/*@}*/
};

#endif

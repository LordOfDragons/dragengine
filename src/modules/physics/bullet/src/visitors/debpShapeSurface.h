/* 
 * Drag[en]gine Bullet Physics Module
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

#ifndef _DEBPSHAPESURFACE_H_
#define _DEBPSHAPESURFACE_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class debpDCollisionVolume;
class debpDCollisionSphere;
class debpDCollisionBox;
class debpDCollisionCylinder;
class debpDCollisionCapsule;



/**
 * \brief Visitor calculating the surface of shapes optionally along a direction.
 */
class debpShapeSurface : public decShapeVisitor{
private:
	decVector pDirection;
	bool pUseDirection;
	
	float pSurface;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create visitor. */
	debpShapeSurface();
	
	/** \brief Clean up visitor. */
	virtual ~debpShapeSurface();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set direction or zero vector to disable. */
	void SetDirection( const decVector &direction );
	
	
	
	/** \brief Surface. */
	inline float GetSurface() const{ return pSurface; }
	
	
	
	/** \brief Reset. */
	void Reset();
	/*@}*/
	
	
	
	/** \name Visiting */
	/*@{*/
	/** \brief Visit shape. */
	virtual void VisitShape( decShape &shape );
	
	/** \brief Visit sphere shape. */
	virtual void VisitShapeSphere( decShapeSphere &sphere );
	
	/** \brief Visit box shape. */
	virtual void VisitShapeBox( decShapeBox &box );
	
	/** \brief Visit cylinder shape. */
	virtual void VisitShapeCylinder( decShapeCylinder &cylinder );
	
	/** \brief Visit capsule shape. */
	virtual void VisitShapeCapsule( decShapeCapsule &capsule );
	
	/** \brief Visit hull shape. */
	virtual void VisitShapeHull( decShapeHull &hull );
	/*@}*/
};

#endif

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

#ifndef _DEBPSHAPETOVOLUME_H_
#define _DEBPSHAPETOVOLUME_H_

#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/math/decMath.h>

class debpDCollisionVolume;
class debpDCollisionSphere;
class debpDCollisionBox;
class debpDCollisionCylinder;
class debpDCollisionCapsule;



/**
 * \brief Bullet Shape to Collision Volume Visitor.
 * 
 * Transforms a shape using a matrix to a collision volume. This is a rather hacky class and
 * serves more as a filler until a better solution is found. Only very few shapes are handled
 * by this class.
 */
class debpShapeToVolume : public decShapeVisitor{
private:
	debpDCollisionSphere *pSphere;
	debpDCollisionBox *pBox;
	debpDCollisionCylinder *pCylinder;
	debpDCollisionCapsule *pCapsule;
	debpDCollisionVolume *pVolume;
	decDMatrix pMatrix;
	
	
	
public:
	/** \name Constructor, Destructor */
	/*@{*/
	/** \brief Create visitor. */
	debpShapeToVolume();
	
	/** \brief Clean up visitor. */
	virtual ~debpShapeToVolume();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	inline const decDMatrix &GetMatrix() const{ return pMatrix; }
	inline debpDCollisionVolume *GetVolume() const{ return pVolume; }
	void SetMatrix( const decDMatrix &matrix );
	
	debpDCollisionVolume *GetVolumeFor( decShape *shape );
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
	
	
	
private:
	void pCleanUp();
};

#endif

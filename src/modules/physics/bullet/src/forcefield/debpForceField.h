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

#ifndef _DEBPFORCEFIELD_H_
#define _DEBPFORCEFIELD_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsForceField.h>

class debpWorld;
class debpFFVortex;
class dePhysicsBullet;

class deForceField;



/**
 * \brief Force field.
 */
class debpForceField : public deBasePhysicsForceField{
private:
	dePhysicsBullet &pBullet;
	const deForceField &pForceField;
	
	debpFFVortex **pVortices;
	int pVortexCount;
	int pVortexSize;
	float pTimeUntilNextVortex;
	
	decVector pDirection;
	decMatrix pVortexMatrix;
	bool pDirtyGeometry;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	debpForceField( dePhysicsBullet &bullet, const deForceField &forceField );
	
	/** \brief Clean up peer. */
	virtual ~debpForceField();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** \brief Force field engine resource. */
	inline const deForceField &GetForceField() const{ return pForceField; }
	
	
	
	/** \brief Force field direction vector. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Vortex matrix. */
	inline const decMatrix &GetVortexMatrix() const{ return pVortexMatrix; }
	
	
	
	/** \brief Update force field. */
	void Update( float elapsed );
	
	
	
	/**
	 * Retrieves the force acting on a given point. The point has to be in the force field
	 * coordinate frame. The result force is stored in the force vector. */
	//void GetForceAtPoint( const decVector &position, decVector &force );
	/*@}*/
	
	
	
	/** \name Vortices */
	/*@{*/
	/** \brief Number of vortices. */
	inline int GetVortexCount() const{ return pVortexCount; }
	
	/** \brief Vortex at the given index. */
	debpFFVortex *GetVortexAt( int index ) const;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Influence area changed. */
	virtual void InfluenceAreaChanged();
	
	/** \brief Radius changed. */
	virtual void RadiusChanged();
	
	/** \brief Exponent changed. */
	virtual void ExponentChanged();
	
	/** \brief Field or application type changed. */
	virtual void TypeChanged();
	
	/** \brief Direction changed. */
	virtual void DirectionChanged();
	
	/** \brief Force changed. */
	virtual void ForceChanged();
	
	/** \brief Direction or force fluctuation changed. */
	virtual void FluctuationChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pUpdateGeometry();
};

#endif

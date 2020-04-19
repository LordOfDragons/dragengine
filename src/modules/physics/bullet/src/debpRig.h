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

#ifndef _DEBPRIG_H_
#define _DEBPRIG_H_

#include <dragengine/systems/modules/physics/deBasePhysicsRig.h>

class dePhysicsBullet;
class deRig;



/**
 * \brief Bullet rig peer.
 */
class debpRig : public deBasePhysicsRig{
private:
	dePhysicsBullet &pBullet;
	const deRig &pRig;
	float pMass;
	float pInvMass;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create bullet rig peer. */
	debpRig( dePhysicsBullet &bullet, const deRig &rig );
	
	/** \brief Clean up rig peer. */
	virtual ~debpRig();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Bullet module. */
	inline dePhysicsBullet &GetBullet() const{ return pBullet; }
	
	/** \brief Engine rig. */
	inline const deRig &GetRig() const{ return pRig; }
	
	/** \brief Rig mass. */
	inline float GetMass() const{ return pMass; }
	
	/** \brief Inverse rig mass. */
	inline float GetInverseMass() const{ return pInvMass; }
	
	
	
private:
	void pCleanUp();
	void pCalculateMass();
};

#endif

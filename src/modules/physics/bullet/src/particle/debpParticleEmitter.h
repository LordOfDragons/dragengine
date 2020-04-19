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

#ifndef _DEBPPROPPARTICLEEMITTER_H_
#define _DEBPPROPPARTICLEEMITTER_H_

#include "LinearMath/btVector3.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/particle/deParticleEmitter.h>
#include <dragengine/systems/modules/physics/deBasePhysicsParticleEmitter.h>

class dePhysicsBullet;
class debpParticleEmitterType;



/**
 * @brief Particle Emitter.
 */
class debpParticleEmitter : public deBasePhysicsParticleEmitter{
private:
	dePhysicsBullet *pBullet;
	deParticleEmitter *pEmitter;
	
	debpParticleEmitterType *pTypes;
	int pTypeCount;
	
	bool pDirtyParameters;
	bool pDirtyGraRequests;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpParticleEmitter( dePhysicsBullet *bullet, deParticleEmitter *emitter );
	/** Cleans up the peer. */
	virtual ~debpParticleEmitter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the force field. */
	inline deParticleEmitter *GetEmitter() const{ return pEmitter; }
	
	/** Retrieves the number of types. */
	inline int GetTypeCount() const{ return pTypeCount; }
	/** Retrieves the type at the given position. */
	debpParticleEmitterType &GetTypeAt( int index );
	const debpParticleEmitterType &GetTypeAt( int index ) const;
	
	/** Update parameters. */
	void UpdateParameters();
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Type count changed. */
	virtual void TypeCountChanged();
	/** Type changed. */
	virtual void TypeChanged( int type );
	/** Requests of the Graphic Module changed. */
	virtual void GraphicModuleRequestsChanged();
	/*@}*/
	
private:
	void pCleanUp();
	void pUpdateTypes();
};

#endif

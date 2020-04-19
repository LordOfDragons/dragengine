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

// include only once
#ifndef _DEBPSMOKEEMITTER_H_
#define _DEBPSMOKEEMITTER_H_

// includes
#include <dragengine/common/math/decMath.h>
#include <dragengine/systems/modules/physics/deBasePhysicsSmokeEmitter.h>

// predefinitions
class debpWorld;
class debpForceField;
class dePhysicsBullet;

class deSmokeEmitter;
struct deSmokeDensityPoint;



/** Density points. */
struct debpSmokeDensityPoint{
	decVector position;
	decVector velocity;
	float size;
};



/**
 * @brief Smoke Emitter.
 *
 * Peer for the smoke emitter resource.
 */
class debpSmokeEmitter : public deBasePhysicsSmokeEmitter{
private:
	dePhysicsBullet *pBullet;
	deSmokeEmitter *pSmokeEmitter;
	debpWorld *pParentWorld;
	
	debpSmokeDensityPoint *pPoints;
	int pPointCount;
	int pPointSize;
	
	//float pCastDensity;
	
	decMatrix pEmitterMatrix;
	bool pDirtyEmitterMatrix;
	
	decVector pGravity;
	
	float pFluctDir;
	float pFluctStr;
	float pFluctTimer;
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new peer. */
	debpSmokeEmitter( dePhysicsBullet *bullet, deSmokeEmitter *smokeEmitter );
	/** Cleans up the peer. */
	virtual ~debpSmokeEmitter();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the module. */
	inline dePhysicsBullet *GetBullet() const{ return pBullet; }
	/** Retrieves the force field. */
	inline deSmokeEmitter *GetSmokeEmitter() const{ return pSmokeEmitter; }
	
	/** Retrieves the parent world. */
	inline debpWorld *GetParentWorld() const{ return pParentWorld; }
	/** Sets the parent world. */
	void SetParentWorld( debpWorld *parentWorld );
	
	/** Retrieves the density points. */
	inline debpSmokeDensityPoint *GetPoints() const{ return pPoints; }
	/** Retrieves the number of density points. */
	inline int GetDensityPointCount() const{ return pPointCount; }
	
	/** Retrieves the emitter matrix. */
	const decMatrix &GetEmitterMatrix();
	
	/** Prepare stepping. */
	void PreparePoints( float elapsed );
	/** Steps the points. */
	void StepPoints( float elapsed );
	
	/** Cast a point. */
	void CastPoint();
	/** Kill a point. */
	void KillPoint( int index );
	
	/** Applies forces caused by a force field. */
	void ApplyForceField( debpForceField *forceField, float elapsed );
	/*@}*/
	
	/** @name Notifications */
	/*@{*/
	/** Emitter position changed. */
	virtual void EmitterPositionChanged();
	/** Emitter orientation changed. */
	virtual void EmitterOrientationChanged();
	/** Emitter casting property changed. */
	virtual void CastPropertyChanged();
	/** Enable casting changed. */
	virtual void EnableCastingChanged();
	/** Component changed. */
	virtual void ComponentChanged();
	/** Cast Texture changed. */
	virtual void CastTextureChanged();
	/** Skin changed. */
	virtual void SkinChanged();
	/** Layer Mask changed. */
	virtual void CollisionFilterChanged();
	/** Gravity changed. */
	virtual void GravityChanged();
	/*@}*/
	
private:
	void pCleanUp();
};

// end of include only once
#endif

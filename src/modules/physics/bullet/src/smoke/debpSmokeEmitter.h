/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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

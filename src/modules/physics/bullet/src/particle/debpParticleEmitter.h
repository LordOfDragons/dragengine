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

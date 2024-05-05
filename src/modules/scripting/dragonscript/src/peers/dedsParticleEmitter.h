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

#ifndef _DEDSPARTICLEEMITTER_H_
#define _DEDSPARTICLEEMITTER_H_

#include <dragengine/systems/modules/scripting/deBaseScriptingParticleEmitterInstance.h>

class deScriptingDragonScript;
class deParticleEmitterInstance;
class dsRealObject;
class dsValue;



/**
 * \brief Particle emitter peer.
 */
class dedsParticleEmitter : public deBaseScriptingParticleEmitterInstance{
private:
	deScriptingDragonScript *pDS;
	deParticleEmitterInstance *pEmitter;
	dsValue *pValCB;
	bool pHasCB;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	dedsParticleEmitter( deScriptingDragonScript *ds, deParticleEmitterInstance *emitter );
	
	/** \brief Clean up peer. */
	virtual ~dedsParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Callback object. */
	dsRealObject *GetCallback() const;
	
	/** \brief Set callback object. */
	void SetCallback( dsRealObject *object );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Last particle cast by this emitter died. */
	virtual void LastParticleDied();
	
	/**
	 * \brief Determine response for a custom particle collision.
	 * 
	 * Update \em collisionInfo with the particle response to use.
	 */
	virtual void CollisionResponse( deCollisionInfo *collisionInfo );
	/*@}*/
};

#endif

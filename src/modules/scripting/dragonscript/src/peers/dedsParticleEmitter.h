/* 
 * Drag[en]gine DragonScript Script Module
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

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

#ifndef _DEBASEPHYSICSPARTICLEEMITTERINSTANCE_H_
#define _DEBASEPHYSICSPARTICLEEMITTERINSTANCE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Physics Particle Emitter Instance Peer.
 */
class DE_DLL_EXPORT deBasePhysicsParticleEmitterInstance{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsParticleEmitterInstance();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsParticleEmitterInstance();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Emitter changed. */
	virtual void EmitterChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Enable casting changed. */
	virtual void EnableCastingChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/**
	 * \brief Ignore colliders changed.
	 * \version 1.7
	 */
	virtual void IgnoreCollidersChanged();
	
	/** \brief Warm up time changed. */
	virtual void WarmUpTimeChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged(int controller);
	
	/** \brief Type changed. */
	virtual void TypeChanged(int type);
	
	/** \brief Reset burst particles. */
	virtual void ResetBurst();
	
	/** \brief Kill all particles. */
	virtual void KillAllParticles();
	/*@}*/
};

#endif

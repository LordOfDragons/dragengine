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

#ifndef _DEBASEGRAPHICPARTICLEEMITTERINSTANCE_H_
#define _DEBASEGRAPHICPARTICLEEMITTERINSTANCE_H_

#include "../../../dragengine_export.h"


/**
 * \brief Graphic Module Particle Emitter Instance Peer
 */
class DE_DLL_EXPORT deBaseGraphicParticleEmitterInstance{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBaseGraphicParticleEmitterInstance();
	
	/** \brief Clean up peer. */
	virtual ~deBaseGraphicParticleEmitterInstance();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Emitter changed. */
	virtual void EmitterChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Reference position changed. */
	virtual void ReferencePositionChanged();
	
	/** \brief Enable casting changed. */
	virtual void EnableCastingChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Warm up time changed. */
	virtual void WarmUpTimeChanged();
	
	/** \brief Burst time changed. */
	virtual void BurstTimeChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** \brief Type changed. */
	virtual void TypeChanged( int type );
	
	/** \brief Type particles changed. */
	virtual void TypeParticlesChanged( int type );
	
	/** \brief Reset burst particles. */
	virtual void ResetBurst();
	
	/** \brief Kill all particles. */
	virtual void KillAllParticles();
	/*@}*/
};

#endif

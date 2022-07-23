/* 
 * Drag[en]gine Game Engine
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
	virtual void ControllerChanged( int controller );
	
	/** \brief Type changed. */
	virtual void TypeChanged( int type );
	
	/** \brief Reset burst particles. */
	virtual void ResetBurst();
	
	/** \brief Kill all particles. */
	virtual void KillAllParticles();
	/*@}*/
};

#endif

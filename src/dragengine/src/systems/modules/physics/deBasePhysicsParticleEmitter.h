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

#ifndef _DEBASEPHYSICSPARTICLEEMITTER_H_
#define _DEBASEPHYSICSPARTICLEEMITTER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Physics Emitter Particle System Peer.
 */
class DE_DLL_EXPORT deBasePhysicsParticleEmitter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsParticleEmitter();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsParticleEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Burst parameters changed. */
	virtual void BurstChanged();
	
	/** \brief Controller count changed. */
	virtual void ControllerCountChanged();
	
	/** \brief Controller changed. */
	virtual void ControllerChanged( int controller );
	
	/** \brief Type count changed. */
	virtual void TypeCountChanged();
	
	/** \brief Type changed. */
	virtual void TypeChanged( int type );
	
	/** \brief Requests of the Graphic Module changed. */
	virtual void GraphicModuleRequestsChanged();
	/*@}*/
};

#endif

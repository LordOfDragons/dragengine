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

#ifndef _DEBASEPHYSICSSMOKEEMITTER_H_
#define _DEBASEPHYSICSSMOKEEMITTER_H_

#include "../../../dragengine_export.h"


/**
 * \brief Smoke Emitter Particle System Peer.
 *
 * Peer for the smoke emitter resource used by the physics system.
 */
class DE_DLL_EXPORT deBasePhysicsSmokeEmitter{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new peer. */
	deBasePhysicsSmokeEmitter();
	
	/** \brief Clean up peer. */
	virtual ~deBasePhysicsSmokeEmitter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Emitter position changed. */
	virtual void EmitterPositionChanged();
	
	/** \brief Emitter orientation changed. */
	virtual void EmitterOrientationChanged();
	
	/** \brief Emitter casting property changed. */
	virtual void CastPropertyChanged();
	
	/** \brief Enable casting changed. */
	virtual void EnableCastingChanged();
	
	/** \brief Component changed. */
	virtual void ComponentChanged();
	
	/** \brief Cast Texture changed. */
	virtual void CastTextureChanged();
	
	/** \brief Skin changed. */
	virtual void SkinChanged();
	
	/** \brief Collision filter changed. */
	virtual void CollisionFilterChanged();
	
	/** \brief Gravity changed. */
	virtual void GravityChanged();
	/*@}*/
};

#endif

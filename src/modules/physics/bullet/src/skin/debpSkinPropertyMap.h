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

#ifndef _DEBPSKINPROPERTYMAP_H_
#define _DEBPSKINPROPERTYMAP_H_

#include <dragengine/common/math/decMath.h>



/**
 * @brief Skin Property Map.
 */
class debpSkinPropertyMap{
public:
	/** Property types. */
	enum ePropertyTypes{
		/** Unsupported. */
		eptUnsupported,
		/** Particle cast size. */
		eptParticleSizeCast,
		/** Particle lifetime size. */
		eptParticleSizeLifetime,
		/** Particle cast color. */
		eptParticleColorCast,
		/** Particle lifetime color. */
		eptParticleColorLifetime,
		/** Particle cast transparency. */
		eptParticleTranspCast,
		/** Particle lifetime transparency. */
		eptParticleTranspLifetime,
		/** Bounciness. */
		eptBounciness,
		/** Number of types. */
		EPT_COUNT
	};
	
private:
	
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new skin property map. */
	debpSkinPropertyMap();
	/** Cleans up the skin property map. */
	~debpSkinPropertyMap();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/** Retrieves the type for the given texture property number. */
	int GetTypeFor( const char *type ) const;
	/*@}*/
};

#endif

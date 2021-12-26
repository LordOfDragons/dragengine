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

#ifndef _DEPARTICLEEMITTERINSTANCETYPE_H_
#define _DEPARTICLEEMITTERINSTANCETYPE_H_

#include "../component/deComponentReference.h"
#include "../skin/dynamic/deDynamicSkinReference.h"
#include "../../common/math/decMath.h"


/**
 * \brief Particle Emitter Instance Type.
 */
class DE_DLL_EXPORT deParticleEmitterInstanceType{
public:
	struct DE_DLL_EXPORT sParticle{ // sizeCast = 6 bytes, sizeDynamic = 22 bytes, sizeTotal = 28 bytes
		float lifetime; // unsigned char possible (1 frame accuracy for 5s particle at 60Hz)
		float positionX; // short possible (~1m accuracy on 50m)
		float positionY; // short possible (~1m accuracy on 50m)
		float positionZ; // short possible (~1m accuracy on 50m)
		float linearVelocity; // unsigned char possible: 0 = 0 m/s, 255 = maxLinSpeed, ev lv=(real-lv squared)
		float angularVelocity; // char possible: -127 = -maxAngSpeed, 0 = 0, 127 = maxAngSpeed, ev av=(real-av squared)
		float castSize; // unsigned char possible: -127 = castLower, 127 = castUpper
		float castEmissivity; // unsigned char possible: -127 = castLower, 127 = castUpper
		unsigned char linearDirectionX; // 0 = -1.0, 0 = 0.0, 255 = 1.0
		unsigned char linearDirectionY; // 0 = -1.0, 0 = 0.0, 255 = 1.0
		unsigned char linearDirectionZ; // 0 = -1.0, 0 = 0.0, 255 = 1.0
		unsigned char rotation; // 0 = 0°, 255 = 360°, step ~ 1.4°
		unsigned char castRed;
		unsigned char castGreen;
		unsigned char castBlue;
		unsigned char castTransparency;
		
		// required images to handle this array in image form:
		// image 1 (rgba, float): lifetime, positionX, positionY, positionZ
		// image 2 (rgba, signed char): linearDirectionX, linearDirectionY, linearDirectionZ, angularVelocity
		// image 3 (rgba, unsigned char): linearVelocity, rotation, castSize, castEmissivity
		// image 4 (rgba, unsigned char): castRed, castGreen, castBlue, castTransparency
	};
	
	
	
private:
	deComponentReference pComponent;
	deDynamicSkinReference pDynamicSkin;
	
	int pParticleCount;
	sParticle *pParticleArray;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new particle emitter instance type. */
	deParticleEmitterInstanceType();
	
	/** \brief Clean up particle emitter instance type. */
	~deParticleEmitterInstanceType();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Component used for casting or NULL. */
	inline deComponent *GetComponent() const{ return pComponent; }
	
	/** \brief Set component used for casting or NULL. */
	void SetComponent( deComponent *component );
	
	/** \brief Dynamic skin or null if not used. */
	inline deDynamicSkin *GetDynamicSkin() const{ return pDynamicSkin; }
	
	/** \brief Set dynamic skin or null if not used. */
	void SetDynamicSkin( deDynamicSkin *dynamicSkin );
	
	/** \brief Particle array or NULL if not set. */
	inline sParticle *GetParticleArray() const{ return pParticleArray; }
	
	/** \brief Number of particles. */
	inline int GetParticleCount() const{ return pParticleCount; }
	
	/**
	 * \brief Set the particle array and number of particles.
	 * 
	 * Used by the Physics Module to set the particle array for the Graphic Module
	 * to render later on.
	 */
	void SetParticleArray( sParticle *particleArray, int count );
	/*@}*/
};

#endif

/* 
 * Drag[en]gine OpenAL Audio Module
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

#ifndef _DEOALSOUNDRAYINTERACTION_H_
#define _DEOALSOUNDRAYINTERACTION_H_

#include <dragengine/common/math/decMath.h>


class deoalAudioThread;
class deoalAComponentTexture;


/**
 * \brief Parameters of a sound ray interacting with a world element.
 * 
 * For use by later calculations requiring additional testing.
 */
class deoalSoundRayInteraction{
private:
	float pDistance;
	decDVector pPoint;
	decDVector pNormal;
	bool pRayEnds;
	
	float pAbsorptionLow;
	float pAbsorptionMedium;
	float pAbsorptionHigh;
	float pTransmissionLow;
	float pTransmissionMedium;
	float pTransmissionHigh;
	float pReflectedLow;
	float pReflectedMedium;
	float pReflectedHigh;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound ray interaction. */
	deoalSoundRayInteraction();
	
	/** \brief Create copy of sound ray interaction. */
	deoalSoundRayInteraction( const deoalSoundRayInteraction &ray );
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Distance in meters to hit point. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set distance in meters to hit point. */
	void SetDistance( float distance );
	
	/** \brief Hit point in world coordinates. */
	inline const decDVector &GetPoint() const{ return pPoint; }
	
	/** \brief Set hit point in world coordinates. */
	void SetPoint( const decDVector &point );
	
	/** \brief Hit normal in world coordinates. */
	inline const decDVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set hit normal in world coordinates. */
	void SetNormal( const decDVector &normal );
	
	/** \brief Ray ends. */
	inline bool GetRayEnds() const{ return pRayEnds; }
	
	/** \brief Set if ray ends. */
	void SetRayEnds( bool rayEnds );
	
	
	
	/** \brief Absorption for low frequencies. */
	inline float GetAbsorptionLow() const{ return pAbsorptionLow; }
	
	/** \brief Absorption for medium frequencies. */
	inline float GetAbsorptionMedium() const{ return pAbsorptionMedium; }
	
	/** \brief Absorption for high frequencies. */
	inline float GetAbsorptionHigh() const{ return pAbsorptionHigh; }
	
	/** \brief Transmission for low frequencies. */
	inline float GetTransmissionLow() const{ return pTransmissionLow; }
	
	/** \brief Transmission for medium frequencies. */
	inline float GetTransmissionMedium() const{ return pTransmissionMedium; }
	
	/** \brief Transmission for high frequencies. */
	inline float GetTransmissionHigh() const{ return pTransmissionHigh; }
	
	/** \brief Reflected for low frequencies. */
	inline float GetReflectedLow() const{ return pReflectedLow; }
	
	/** \brief Reflected for medium frequencies. */
	inline float GetReflectedMedium() const{ return pReflectedMedium; }
	
	/** \brief Reflected for high frequencies. */
	inline float GetReflectedHigh() const{ return pReflectedHigh; }
	
	/**
	 * \brief Set parameters from component texture using distance.
	 * 
	 * Absorption is set to the texture absorption values.
	 * Transmission is set to "1 - absorption" scaled by texture transmission over distance.
	 * Reflected is set to "1 - absorption - trannsmission"
	 * RayEnds is set to true if all transmission values are 0.
	 */
	void SetParameters( const deoalAComponentTexture &texture, float distance );
	
	/**
	 * \brief Set parameters from component texture using infinite distance.
	 * 
	 * Absorption is set to the texture absorption values.
	 * Transmission is set to 0.
	 * Reflected is set to "1 - absorption"
	 * RayEnds is set to true.
	 * 
	 * Same as if texture has all transmission ranges set to 0.
	 */
	void SetParameters( const deoalAComponentTexture &texture );
	
	
	
	/** \brief Debug print. */
	void DebugPrint( deoalAudioThread &audioThread, const char *prefix );
	
	
	
	/** \brief Assign element. */
	deoalSoundRayInteraction &operator=( const deoalSoundRayInteraction &element );
	/*@}*/
};

#endif

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

#ifndef _DEOALSOUNDRAYSEGMENT_H_
#define _DEOALSOUNDRAYSEGMENT_H_

#include <dragengine/common/math/decMath.h>



/**
 * \brief Parameters of a sound ray segment.
 */
class deoalSoundRaySegment{
private:
	decVector pPosition;
	decVector pDirection;
	decVector pNormal;
	float pLength;
	float pDistance;
	float pGainLow;
	float pGainMedium;
	float pGainHigh;
	float pAbsorptionSumLow;
	float pAbsorptionSumMedium;
	float pAbsorptionSumHigh;
	int pBounceCount;
	int pTransmittedCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound ray segment. */
	deoalSoundRaySegment();
	
	/** \brief Create copy of sound ray segment. */
	deoalSoundRaySegment( const deoalSoundRaySegment &segment);
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Origin. */
	inline const decVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set origin. */
	void SetPosition( const decVector &position );
	
	/** \brief Normalized direction. */
	inline const decVector &GetDirection() const{ return pDirection; }
	
	/** \brief Set normalized direction. */
	void SetDirection( const decVector &direction );
	
	/** \brief Normal. */
	inline const decVector &GetNormal() const{ return pNormal; }
	
	/** \brief Set normal. */
	void SetNormal( const decVector &normal );
	
	/** \brief Length. */
	inline float GetLength() const{ return pLength; }
	
	/** \brief Set length. */
	void SetLength( float length );
	
	/** \brief Attenuation distance at segment origin. */
	inline float GetDistance() const{ return pDistance; }
	
	/** \brief Set attenuation distance at segment origin. */
	void SetDistance( float distance );
	
	
	
	/** \brief Gain for low frequencies at segment origin. */
	inline float GetGainLow() const{ return pGainLow; }
	
	/** \brief Set gain for low frequencies at segment origin. */
	void SetGainLow( float gain );
	
	/** \brief Gain for medium frequencies at segment origin. */
	inline float GetGainMedium() const{ return pGainMedium; }
	
	/** \brief Set gain for medium frequencies at segment origin. */
	void SetGainMedium( float gain );
	
	/** \brief Gain for high frequencies at segment origin. */
	inline float GetGainHigh() const{ return pGainHigh; }
	
	/** \brief Set gain for high frequencies at segment origin. */
	void SetGainHigh( float gain );
	
	
	
	/** \brief Absorption sum for low frequencies at segment origin. */
	inline float GetAbsorptionSumLow() const{ return pAbsorptionSumLow; }
	
	/** \brief Set absorption sum for low frequencies at segment origin. */
	void SetAbsorptionSumLow( float absorption );
	
	/** \brief Absorption sum for medium frequencies at segment origin. */
	inline float GetAbsorptionSumMedium() const{ return pAbsorptionSumMedium; }
	
	/** \brief Set absorption sum for medium frequencies at segment origin. */
	void SetAbsorptionSumMedium( float absorption );
	
	/** \brief Absorption sum for high frequencies at segment origin. */
	inline float GetAbsorptionSumHigh() const{ return pAbsorptionSumHigh; }
	
	/** \brief Set absorption sum for high frequencies at segment origin. */
	void SetAbsorptionSumHigh( float absorption );
	
	
	
	/** \brief Number of times the ray bounced at the start of the segment. */
	inline int GetBounceCount() const{ return pBounceCount; }
	
	/** \brief Set number of times the ray bounced at the start of the segment. */
	void SetBounceCount( int count );
	
	/** \brief Number of times the ray transmitted at the start of the segment. */
	inline int GetTransmittedCount() const{ return pTransmittedCount; }
	
	/** \brief Set number of times the ray transmitted at the start of the segment. */
	void SetTransmittedCount( int count );
	
	
	
	/** \brief Set from other segment. */
	deoalSoundRaySegment &operator=( const deoalSoundRaySegment &segment );
	/*@}*/
};

#endif

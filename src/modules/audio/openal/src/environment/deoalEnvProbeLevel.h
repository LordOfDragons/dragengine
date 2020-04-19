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

#ifndef _DEOALENVPROBELEVEL_H_
#define _DEOALENVPROBELEVEL_H_

#include "raytrace/deoalSoundRayList.h"
#include "raytrace/deoalSoundRayInteractionList.h"

#include <dragengine/common/math/decMath.h>



/**
 * \brief Environment probe level.
 * 
 * Stores the result of an environment probe for a specific refinement level. All values
 * except lists are stored with the accumulated value of all previous levels and this level.
 * Listings like the sound ray and sound interaction list contain only the information from
 * this level.
 */
class deoalEnvProbeLevel{
private:
	deoalSoundRayList pSoundRayList;
	deoalSoundRayInteractionList pSRIList;
	int pRayCount;
	float pRayOpeningAngle;
	
	float pReverberationTimeMedium;
	float pReverberationTimeLow;
	float pReverberationTimeHigh;
	float pEchoDelay;
	
	float pRoomVolume;
	float pRoomSurface;
	float pRoomSabineLow;
	float pRoomSabineMedium;
	float pRoomSabineHigh;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe level. */
	deoalEnvProbeLevel();
	
	/** \brief Clean up environment probe level. */
	~deoalEnvProbeLevel();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Traced sound ray list. */
	inline deoalSoundRayList &GetSoundRayList(){ return pSoundRayList; }
	inline const deoalSoundRayList &GetSoundRayList() const{ return pSoundRayList; }
	
	/** \brief First reflection sound ray interaction list. */
	inline deoalSoundRayInteractionList &GetSRIList(){ return pSRIList; }
	inline const deoalSoundRayInteractionList &GetSRIList() const{ return pSRIList; }
	
	/** \brief Number of rays used to calculate probe. */
	inline int GetRayCount() const{ return pRayCount; }
	
	/** \brief Set number of rays used to calculate probe. */
	void SetRayCount( int rayCount );
	
	/** \brief Ray beam opening angle used for calculating probe. */
	inline float GetRayOpeningAngle() const{ return pRayOpeningAngle; }
	
	/** \brief Set ray beam opening angle used for calculating probe. */
	void SetRayOpeningAngle( float angle );
	
	
	
	/** \brief Reverberation time in seconds for medium frequencies. */
	inline float GetReverberationTimeMedium() const{ return pReverberationTimeMedium; }
	
	/** \brief Reverberation time in seconds for low frequencies. */
	inline float GetReverberationTimeLow() const{ return pReverberationTimeLow; }
	
	/** \brief Reverberation time in seconds for high frequencies. */
	inline float GetReverberationTimeHigh() const{ return pReverberationTimeHigh; }
	
	/** \brief Set reverberation. */
	void SetReverberationTime( float timeLow, float timeMedium, float timeHigh );
	
	/** \brief Echo delay. */
	inline float GetEchoDelay() const{ return pEchoDelay; }
	
	/** \brief Set echo. */
	void SetEcho( float delay );
	
	
	
	/** \brief Room volume. */
	inline float GetRoomVolume() const{ return pRoomVolume; }
	
	/** \brief Room surface. */
	inline float GetRoomSurface() const{ return pRoomSurface; }
	
	/** \brief Room sabine for low frequencies. */
	inline float GetRoomSabineLow() const{ return pRoomSabineLow; }
	
	/** \brief Room sabine for medium frequencies. */
	inline float GetRoomSabineMedium() const{ return pRoomSabineMedium; }
	
	/** \brief Room sabine for high frequencies. */
	inline float GetRoomSabineHigh() const{ return pRoomSabineHigh; }
	
	/** \brief Set room indirect sound parameters. */
	void SetRoom( float volume, float surface, float sabineLow, float sabineMedium, float sabineHigh );
	/*@}*/
};

#endif


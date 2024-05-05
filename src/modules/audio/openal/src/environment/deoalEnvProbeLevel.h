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


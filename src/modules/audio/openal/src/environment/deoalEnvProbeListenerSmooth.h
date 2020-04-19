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

#ifndef _DEOALENVPROBELISTENERSMOOTH_H_
#define _DEOALENVPROBELISTENERSMOOTH_H_

#include <dragengine/common/math/smooth/decSmoothFloat.h>
#include <dragengine/common/math/smooth/decSmoothVector.h>


class deoalEnvProbeListener;


/**
 * \brief Environment probe listener with smoothed parameters.
 * 
 * Supports blending smoothly over to new listener values.
 */
class deoalEnvProbeListenerSmooth{
private:
	decSmoothFloat pReflectedLow;
	decSmoothFloat pReflectedMedium;
	decSmoothFloat pReflectedHigh;
	decSmoothFloat pReflectionDelay;
	decSmoothVector pReflectionPan;
	decSmoothFloat pReverberationGainLow;
	decSmoothFloat pReverberationGainMedium;
	decSmoothFloat pReverberationGainHigh;
	decSmoothFloat pReverberationDelay;
	decSmoothVector pReverberationPan;
	decSmoothFloat pReverberationTimeLow;
	decSmoothFloat pReverberationTimeMedium;
	decSmoothFloat pReverberationTimeHigh;
	decSmoothFloat pEchoDelay;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe listener. */
	deoalEnvProbeListenerSmooth();
	
	/** \brief Create environment probe listener. */
	deoalEnvProbeListenerSmooth( const deoalEnvProbeListenerSmooth &listener );
	
	/** \brief Clean up environment probe listener. */
	~deoalEnvProbeListenerSmooth();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Reflected percentage for low frequencies. */
	inline float GetReflectedLow() const{ return pReflectedLow.GetValue(); }
	
	/** \brief Reflected percentage for medium frequencies. */
	inline float GetReflectedMedium() const{ return pReflectedMedium.GetValue(); }
	
	/** \brief Reflected percentage for high frequencies. */
	inline float GetReflectedHigh() const{ return pReflectedHigh.GetValue(); }
	
	/** \brief Reflection delay in seconds. */
	inline float GetReflectionDelay() const{ return pReflectionDelay.GetValue(); }
	
	/** \brief Reflection pan direction. */
	inline const decVector &GetReflectionPan() const{ return pReflectionPan.GetValue(); }
	
	/** \brief Reverberation gain for low frequencies. */
	inline float GetReverberationGainLow() const{ return pReverberationGainLow.GetValue(); }
	
	/** \brief Reverberation gain for medium frequencies. */
	inline float GetReverberationGainMedium() const{ return pReverberationGainMedium.GetValue(); }
	
	/** \brief Reverberation gain for high frequencies. */
	inline float GetReverberationGainHigh() const{ return pReverberationGainHigh.GetValue(); }
	
	/** \brief Reverberation delay in seconds. */
	inline float GetReverberationDelay() const{ return pReverberationDelay.GetValue(); }
	
	/** \brief Reverberation pan direction. */
	inline const decVector &GetReverberationPan() const{ return pReverberationPan.GetValue(); }
	
	/** \brief Reverberation time in seconds for medium frequencies. */
	inline float GetReverberationTimeMedium() const{ return pReverberationTimeMedium.GetValue(); }
	
	/** \brief Reverberation time in seconds for low frequencies. */
	inline float GetReverberationTimeLow() const{ return pReverberationTimeLow.GetValue(); }
	
	/** \brief Reverberation time in seconds for high frequencies. */
	inline float GetReverberationTimeHigh() const{ return pReverberationTimeHigh.GetValue(); }
	
	
	
	/** \brief Set current and goal from listener. */
	void Set( const deoalEnvProbeListener &listener );
	
	/** \brif Set goal from listener. */
	void SetGoal( const deoalEnvProbeListener &listener );
	
	/** \brief Update values. */
	void Update( float elapsed );
	
	/** \brief Update listener with current values. */
	void AssignTo( deoalEnvProbeListener &listener );
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalEnvProbeListenerSmooth &operator=( const deoalEnvProbeListenerSmooth &listener );
	/*@}*/
};

#endif


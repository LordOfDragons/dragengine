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
	deoalEnvProbeListenerSmooth(const deoalEnvProbeListenerSmooth &listener);
	
	/** \brief Clean up environment probe listener. */
	~deoalEnvProbeListenerSmooth();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Reflected percentage for low frequencies. */
	inline float GetReflectedLow() const{return pReflectedLow.GetValue();}
	
	/** \brief Reflected percentage for medium frequencies. */
	inline float GetReflectedMedium() const{return pReflectedMedium.GetValue();}
	
	/** \brief Reflected percentage for high frequencies. */
	inline float GetReflectedHigh() const{return pReflectedHigh.GetValue();}
	
	/** \brief Reflection delay in seconds. */
	inline float GetReflectionDelay() const{return pReflectionDelay.GetValue();}
	
	/** \brief Reflection pan direction. */
	inline const decVector &GetReflectionPan() const{return pReflectionPan.GetValue();}
	
	/** \brief Reverberation gain for low frequencies. */
	inline float GetReverberationGainLow() const{return pReverberationGainLow.GetValue();}
	
	/** \brief Reverberation gain for medium frequencies. */
	inline float GetReverberationGainMedium() const{return pReverberationGainMedium.GetValue();}
	
	/** \brief Reverberation gain for high frequencies. */
	inline float GetReverberationGainHigh() const{return pReverberationGainHigh.GetValue();}
	
	/** \brief Reverberation delay in seconds. */
	inline float GetReverberationDelay() const{return pReverberationDelay.GetValue();}
	
	/** \brief Reverberation pan direction. */
	inline const decVector &GetReverberationPan() const{return pReverberationPan.GetValue();}
	
	/** \brief Reverberation time in seconds for medium frequencies. */
	inline float GetReverberationTimeMedium() const{return pReverberationTimeMedium.GetValue();}
	
	/** \brief Reverberation time in seconds for low frequencies. */
	inline float GetReverberationTimeLow() const{return pReverberationTimeLow.GetValue();}
	
	/** \brief Reverberation time in seconds for high frequencies. */
	inline float GetReverberationTimeHigh() const{return pReverberationTimeHigh.GetValue();}
	
	
	
	/** \brief Set current and goal from listener. */
	void Set(const deoalEnvProbeListener &listener);
	
	/** \brif Set goal from listener. */
	void SetGoal(const deoalEnvProbeListener &listener);
	
	/** \brief Update values. */
	void Update(float elapsed);
	
	/** \brief Update listener with current values. */
	void AssignTo(deoalEnvProbeListener &listener);
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalEnvProbeListenerSmooth &operator=(const deoalEnvProbeListenerSmooth &listener);
	/*@}*/
};

#endif


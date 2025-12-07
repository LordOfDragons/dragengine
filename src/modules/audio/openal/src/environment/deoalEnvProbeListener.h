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

#ifndef _DEOALENVPROBELISTENER_H_
#define _DEOALENVPROBELISTENER_H_

#include "raytrace/deoalImpulseResponse.h"
#include "raytrace/deoalEnergyHistogram.h"

#include <dragengine/common/math/decMath.h>



/**
 * \brief Environment probe listener parameters.
 * 
 * Stores the result of an environment probe for a specific listener position. Each probe can
 * store parameters for mutliple listener positions. This allows to cache calculated listener
 * parameters.
 */
class deoalEnvProbeListener{
private:
	float pReflectedLow;
	float pReflectedMedium;
	float pReflectedHigh;
	float pReflectionDelay;
	decVector pReflectionPan;
	float pReverberationGainLow;
	float pReverberationGainMedium;
	float pReverberationGainHigh;
	float pReverberationDelay;
	decVector pReverberationPan;
	
	float pReverberationTimeLow;
	float pReverberationTimeMedium;
	float pReverberationTimeHigh;
	float pEchoDelay;
	
	deoalImpulseResponse pImpulseResponse;
	deoalEnergyHistogram pEnergyHistogram;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create environment probe listener. */
	deoalEnvProbeListener();
	
	/** \brief Create environment probe listener. */
	deoalEnvProbeListener(const deoalEnvProbeListener &listener);
	
	/** \brief Clean up environment probe listener. */
	~deoalEnvProbeListener();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Reflected percentage for low frequencies. */
	inline float GetReflectedLow() const{return pReflectedLow;}
	
	/** \brief Reflected percentage for medium frequencies. */
	inline float GetReflectedMedium() const{return pReflectedMedium;}
	
	/** \brief Reflected percentage for high frequencies. */
	inline float GetReflectedHigh() const{return pReflectedHigh;}
	
	/** \brief Reflection delay in seconds. */
	inline float GetReflectionDelay() const{return pReflectionDelay;}
	
	/** \brief Set reflected. */
	void SetReflected(float gainLow, float gainMedium, float gainHigh, float delay);
	
	/** \brief Reflection pan direction. */
	inline const decVector &GetReflectionPan() const{return pReflectionPan;}
	
	/** \brief Set reflection pan direction. */
	void SetReflectionPan(const decVector &pan);
	
	/** \brief Reverberation gain for low frequencies. */
	inline float GetReverberationGainLow() const{return pReverberationGainLow;}
	
	/** \brief Reverberation gain for medium frequencies. */
	inline float GetReverberationGainMedium() const{return pReverberationGainMedium;}
	
	/** \brief Reverberation gain for high frequencies. */
	inline float GetReverberationGainHigh() const{return pReverberationGainHigh;}
	
	/** \brief Reverberation delay in seconds. */
	inline float GetReverberationDelay() const{return pReverberationDelay;}
	
	/** \brief Set reverberation. */
	void SetReverberation(float gainLow, float gainMedium, float gainHigh, float delay);
	
	/** \brief Reverberation pan direction. */
	inline const decVector &GetReverberationPan() const{return pReverberationPan;}
	
	/** \brief Set reverberation pan direction. */
	void SetReverberationPan(const decVector &pan);
	
	
	
	/** \brief Reverberation time in seconds for medium frequencies. */
	inline float GetReverberationTimeMedium() const{return pReverberationTimeMedium;}
	
	/** \brief Reverberation time in seconds for low frequencies. */
	inline float GetReverberationTimeLow() const{return pReverberationTimeLow;}
	
	/** \brief Reverberation time in seconds for high frequencies. */
	inline float GetReverberationTimeHigh() const{return pReverberationTimeHigh;}
	
	/** \brief Set reverberation. */
	void SetReverberationTime(float timeLow, float timeMedium, float timeHigh);
	
	/** \brief Echo delay. */
	inline float GetEchoDelay() const{return pEchoDelay;}
	
	/** \brief Set echo. */
	void SetEcho(float delay);
	
	/** \brief Impulse response. */
	inline deoalImpulseResponse &GetImpulseResponse(){return pImpulseResponse;}
	inline const deoalImpulseResponse &GetImpulseResponse() const{return pImpulseResponse;}
	
	/** \brief Energy histogram. */
	inline deoalEnergyHistogram &GetEnergyHistogram(){return pEnergyHistogram;}
	inline const deoalEnergyHistogram &GetEnergyHistogram() const{return pEnergyHistogram;}
	
	
	
	/** \brif Apply scaled parameters. */
	void AddScaled(const deoalEnvProbeListener &listener, float factor);
	
	/** \brif Normalize. */
	void Normalize(float factor);
	
	/** \brif Clear. */
	void Clear();
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalEnvProbeListener &operator=(const deoalEnvProbeListener &listener);
	
	/** \brief Add. */
	deoalEnvProbeListener &operator+=(const deoalEnvProbeListener &listener);
	/*@}*/
};

#endif


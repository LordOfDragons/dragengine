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

#ifndef _DEOALIMPULSERESPONSE_H_
#define _DEOALIMPULSERESPONSE_H_

#include <dragengine/common/collection/decTList.h>

class deoalSoundRayInteraction;



/**
 * Impulse response.
 * 
 * List of time/values whereas values can have different meaning depending on the usage of the
 * impulse response. The list is greedy to avoid allocations. Entries can be added to the
 * impulse response or inserted. Adding produces an unsorted impulse response. Inserting
 * produces an impulse response sorted by time.
 */
class deoalImpulseResponse{
public:
	/** Impulse entry. */
	struct sImpulse{
		float time;
		float low;
		float medium;
		float high;
	};
	
	
	
private:
	decTList<sImpulse> pImpulses;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create impulse response. */
	deoalImpulseResponse();
	
	/** Create copy of impulse response. */
	deoalImpulseResponse(const deoalImpulseResponse &response);
	
	/** Clean up impulse response. */
	~deoalImpulseResponse();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Impulses. */
	inline const decTList<sImpulse> &GetImpulses() const{ return pImpulses; }
	
	/** Add impulse producing unsorted impulse response. */
	void Add(float time, float low, float medium, float high);
	
	/** Add impulse response producing unsorted impulse response. */
	void Add(const deoalImpulseResponse &response);
	
	/** Insert impulse producing impulse response sorted by time. */
	void Insert(float time, float low, float medium, float high);
	
	/** Insert impulse response producing impulse response sorted by time. */
	void Insert(const deoalImpulseResponse &response);
	
	/** Insert scaled impulse response producing impulse response sorted by time. */
	void InsertScaled(const deoalImpulseResponse &response, float scale);
	
	/** Clear impulse response. */
	void Clear();
	
	/** Reserve space in the array to make future Add/Insert more efficient. */
	void ReserveSize(int size);
	
	
	
	/** Square impulse response. */
	void Square();
	
	/** Convert from pressure to SPL[dB] (sound pressure level). */
	void PressureToSPL();
	
	/** Convert from energy/intensity to SIL[dB] (sound intensity level). */
	void IntensityToSIL();
	
	/** Convert from energy/intensity to pressure(OpenAL). */
	void IntensityToPressure();
	
	/** Convert from pressure(OpenAL) to energy/intensity. */
	void PressureToIntensity();
	
	/**
	 * Schroedinger backwards integration.
	 * 
	 * Impulse response has be energy/intensity and will become sound power level (dB).
	 * If impulse response is pressure call Square() first.
	 */
	void BackwardIntegrate();
	
	/**
	 * Calculate slope in dB/s from first to last impulse.
	 * 
	 * Valid only for impulse response after BackwardIntegrate() has been called.
	 * The returned impulse contains these values:
	 * - time: time difference in seconds between first and last impulse.
	 * - low: slope in dB/s from first to last for low frequencies.
	 * - medium: slope in dB/s from first to last for medium frequencies.
	 * - high: slope in dB/s from first to last for hight frequencies.
	 * 
	 * If the number of impulses is less than 2 the result is all set to 0.
	 */
	sImpulse Slopes() const;
	
	/**
	 * Calculate reverberation time from first to last impulse.
	 * 
	 * Valid only for impulse response after BackwardIntegrate() has been called.
	 * The returned impulse contains these values:
	 * - time: time difference in seconds between first and last impulse.
	 * - low: reverberation time from first to last for low frequencies.
	 * - medium: reverberation time from first to last for medium frequencies.
	 * - high: reverberation time from first to last for hight frequencies.
	 * 
	 * If the number of impulses is less than 2 the result is all set to 0.
	 * 
	 * The reverberation time is calculated as "dropDb / slope[dB/s]" with default dropDb = 60.
	 */
	sImpulse ReverberationTime(float dropDb = 60.0f) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** Assign. */
	deoalImpulseResponse &operator=(const deoalImpulseResponse &list);
	
	/** Scale values by factor. */
	deoalImpulseResponse &operator*=(float factor);
	/*@}*/
	
	
	
private:
	void pInsert(float time, float low, float medium, float high);
};

#endif

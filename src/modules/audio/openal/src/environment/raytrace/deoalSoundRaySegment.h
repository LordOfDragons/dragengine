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

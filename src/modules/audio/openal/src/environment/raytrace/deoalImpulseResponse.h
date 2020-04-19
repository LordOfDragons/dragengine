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

#ifndef _DEOALIMPULSERESPONSE_H_
#define _DEOALIMPULSERESPONSE_H_

class deoalSoundRayInteraction;



/**
 * \brief Impulse response.
 * 
 * List of time/values whereas values can have different meaning depending on the usage of the
 * impulse response. The list is greedy to avoid allocations. Entries can be added to the
 * impulse response or inserted. Adding produces an unsorted impulse response. Inserting
 * produces an impulse response sorted by time.
 */
class deoalImpulseResponse{
public:
	/** \brief Impulse entry. */
	struct sImpulse{
		float time;
		float low;
		float medium;
		float high;
	};
	
	
	
private:
	sImpulse *pImpulses;
	int pCount;
	int pSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create impulse response. */
	deoalImpulseResponse();
	
	/** \brief Create copy of impulse response. */
	deoalImpulseResponse( const deoalImpulseResponse &response );
	
	/** \brief Clean up impulse response. */
	~deoalImpulseResponse();
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of impulses. */
	inline int GetCount() const{ return pCount; }
	
	/** \brief Impulse at index. */
	const sImpulse &GetAt( int index ) const;
	
	/** \brief Add impulse producing unsorted impulse response. */
	void Add( float time, float low, float medium, float high );
	
	/** \brief Add impulse response producing unsorted impulse response. */
	void Add( const deoalImpulseResponse &response );
	
	/** \brief Insert impulse producing impulse response sorted by time. */
	void Insert( float time, float low, float medium, float high );
	
	/** \brief Insert impulse response producing impulse response sorted by time. */
	void Insert( const deoalImpulseResponse &response );
	
	/** \brief Insert scaled impulse response producing impulse response sorted by time. */
	void InsertScaled( const deoalImpulseResponse &response, float scale );
	
	/** \brief Clear impulse response. */
	void Clear();
	
	/** \brief Reserve space in the array to make future Add/Insert more efficient. */
	void ReserveSize( int size );
	
	
	
	/** \brief Square impulse response. */
	void Square();
	
	/** \brief Convert from pressure to SPL[dB] (sound pressure level). */
	void PressureToSPL();
	
	/** \brief Convert from energy/intensity to SIL[dB] (sound intensity level). */
	void IntensityToSIL();
	
	/** \brief Convert from energy/intensity to pressure(OpenAL). */
	void IntensityToPressure();
	
	/** \brief Convert from pressure(OpenAL) to energy/intensity. */
	void PressureToIntensity();
	
	/**
	 * \brief Schroedinger backwards integration.
	 * 
	 * Impulse response has be energy/intensity and will become sound power level (dB).
	 * If impulse response is pressure call Square() first.
	 */
	void BackwardIntegrate();
	
	/**
	 * \brief Calculate slope in dB/s from first to last impulse.
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
	 * \brief Calculate reverberation time from first to last impulse.
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
	sImpulse ReverberationTime( float dropDb = 60.0f ) const;
	/*@}*/
	
	
	
	/** \name Operators */
	/*@{*/
	/** \brief Assign. */
	deoalImpulseResponse &operator=( const deoalImpulseResponse &list );
	
	/** \brief Scale values by factor. */
	deoalImpulseResponse &operator*=( float factor );
	/*@}*/
	
	
	
private:
	void pResize( int size );
	void pInsert( float time, float low, float medium, float high );
};

#endif

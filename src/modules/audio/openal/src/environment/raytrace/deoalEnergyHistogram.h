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

#ifndef _DEOALENERGYHISTOGRAM_H_
#define _DEOALENERGYHISTOGRAM_H_

#include <dragengine/common/math/decMath.h>

class deoalImpulseResponse;



/**
 * \brief Energy histogram.
 * 
 * Histogram composes of frequency bands each storing the same number of slots. The slot size
 * is the time-delta mapping to the slot.
 */
class deoalEnergyHistogram{
private:
	int pSlotCount;
	int pBandCount;
	float pTime;
	float pSlotTime;
	float *pEntries;
	int pEntryCount;
	float pInvSlotTime;
	float pHalfSlotSize;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create empty histogram. */
	deoalEnergyHistogram();
	
	/** \brief Create histogram. */
	deoalEnergyHistogram( int slotCount, int bandCount, float time );
	
	/** \brief Create copy of histogram. */
	deoalEnergyHistogram( const deoalEnergyHistogram &histogram );
	/*@}*/
	
	
	
	/** \name Manegement */
	/*@{*/
	/** \brief Number of slots. */
	inline int GetSlotCount() const{ return pSlotCount; }
	
	/** \brief Number of frequency bands. */
	inline int GetBandCount() const{ return pBandCount; }
	
	/**
	 * \brief Set histogram parameters.
	 * 
	 * If the parameters are the same as the parameters currently set in the histogram the
	 * histogram stays unchanged.
	 */
	void SetParameters( int slotCount, int bandCount, float time );
	
	/** \brief Set histogram time. */
	/** \brief Histogram time. */
	inline float GetTime() const{ return pTime; }
	
	/** \brief Slot time in seconds. */
	inline float GetSlotTime() const{ return pSlotTime; }
	
	/** \brief Clear histogram. */
	void Clear();
	
	/**
	 * \brief Add energy.
	 * \param[in] time Time in seconds used to find the right slot. If the time is 0 or less
	 *                 slot 0 is used. If the time is larger than the histogram time the
	 *                 entries are ignored and the histogram stays unchanged.
	 * \param[in] energies Array of GetBandCount() energy values to insert.
	 */
	void Add( float time, const float *energies );
	
	/**
	 * \brief Add entries from other histogram.
	 * \throws deeInvalidParam Slot or band count is not the same.
	 */
	void Add( const deoalEnergyHistogram &histogram );
	
	/**
	 * \brief Add entries from impulse response.
	 * \throws deeInvalidParam Band count is not 3.
	 */
	void Add( const deoalImpulseResponse &impulseResponse );
	
	/**
	 * \brief Add energy.
	 * \param[in] time Time in seconds used to find the right slot. If the time is 0 or less
	 *                 slot 0 is used. If the time is larger than the histogram time the
	 *                 entries are ignored and the histogram stays unchanged.
	 * \param[in] energies Array of GetBandCount() energy values to insert.
	 */
	void AddMax( float time, const float *energies );
	
	/**
	 * \brief Add entries from other histogram.
	 * \throws deeInvalidParam Slot or band count is not the same.
	 */
	void AddMax( const deoalEnergyHistogram &histogram );
	
	/**
	 * \brief Add entries from other histogram scaled by factor.
	 * \throws deeInvalidParam Slot or band count is not the same.
	 */
	void BlendAdd( const deoalEnergyHistogram &histogram );
	
	/**
	 * \brief Add entries from other histogram scaled by factor.
	 * \throws deeInvalidParam Slot or band count is not the same.
	 */
	void BlendAddScaled( const deoalEnergyHistogram &histogram, float factor );
	
	/**
	 * \brief Histogram values.
	 * 
	 * Linear array of values layed out by bands starting with band 0 without padding.
	 */
	inline const float *GetEntries() const{ return pEntries; }
	
	/**
	 * \brief Integrate band energies.
	 * \param[out] integrations Array of GetBandCount() floats updated with the integrations.
	 */
	void Integrate( float *integrations ) const;
	
	
	/**
	 * \brief Set from other histogram.
	 * \note Calls SetParameters() with histogram parameters before copying entries.
	 */
	deoalEnergyHistogram &operator=( const deoalEnergyHistogram &histogram );
	
	/** \brief Scale entries by factor. */
	deoalEnergyHistogram &operator*=( float factor );
	/*@}*/
};

#endif

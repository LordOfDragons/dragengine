/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASESOUNDINFOS_H_
#define _DEBASESOUNDINFOS_H_

#include "../deBaseModule.h"


/**
 * \brief Sound Module Sound Information.
 *
 * Provides information for a sound to loading afterwards. Can be used
 * by the sound module to store additional data structures needed for loading
 * in a subclasses object.
 */
class deBaseSoundInfo{
private:
	int pBytesPerSample;
	int pSampleCount;
	int pSampleRate;
	int pChannelCount;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound info. */
	deBaseSoundInfo();
	
	/** \brief Clean up sound info object. */
	~deBaseSoundInfo();
	/*@}*/
	
	
	
	/** \name Sound Information */
	/*@{*/
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Set bytes per sample. */
	void SetBytesPerSample( int bytesPerSample );
	
	/** \brief Number of samples. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Set number of samples. */
	void SetSampleCount( int sampleCount );
	
	/** \brief Number of channels. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Set number of channels. */
	void SetChannelCount( int channelCount );
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Set sample rate. */
	void SetSampleRate( int sampleRate );
	/*@}*/
};

#endif

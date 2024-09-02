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
class DE_DLL_EXPORT deBaseSoundInfo{
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

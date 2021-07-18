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

#ifndef _DESOUND_H_
#define _DESOUND_H_

#include "../deFileResource.h"


class deSoundManager;
class deBaseAudioSound;
class deBaseSynthesizerSound;


/**
 * \brief Sound resource.
 *
 * Sounds represent sampled data to play back with speakers or used with synthesizers.
 * Sound resources can be used for short sound effect or long music files. To obtain
 * the samples modules use sound decoders. It is at the discretion of the involved
 * modules if they deem the sound file short enough to cache in memory or if they
 * use the samples in a streaming way.
 */
class deSound : public deFileResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSound> Ref;
	
	
	
private:
	int pBytesPerSample;
	int pSampleCount;
	int pChannelCount;
	int pSampleRate;
	float pPlayTime;
	
	deBaseAudioSound *pPeerAudio;
	deBaseSynthesizerSound *pPeerSynthesizer;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound. */
	deSound( deSoundManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime, int bytesPerSample, int sampleRate,
		int sampleCount, int channelCount );
	
	/**
	 * \brief Create sound for internal loading.
	 * \warning This is a special internal constructor. Never ever call this on your own!
	 */
	deSound( deSoundManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime );
	
protected:
	/**
	 * \brief Clean up sound object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSound();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Number of samples. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Number of channels. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Play time in seconds. */
	inline float GetPlayTime() const{ return pPlayTime; }
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Audio system peer object. */
	inline deBaseAudioSound *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer object. */
	void SetPeerAudio( deBaseAudioSound *peer );
	
	/** \brief Synthesizer system peer object. */
	inline deBaseSynthesizerSound *GetPeerSynthesizer() const{ return pPeerSynthesizer; }
	
	/** \brief Set synthesizer system peer object. */
	void SetPeerSynthesizer( deBaseSynthesizerSound *peer );
	/*@}*/
	
	
	
	/** \name Special */
	/*@{*/
	/**
	 * \brief Finalize construction after asynchronous loading.
	 * \warning This is a special internal function. Never ever call this on your own!
	 */
	void FinalizeConstruction( int bytesPerSample, int sampleRate,
		int sampleCount, int channelCount );
	/*@}*/
};

#endif

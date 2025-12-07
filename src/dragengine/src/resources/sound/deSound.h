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
class DE_DLL_EXPORT deSound : public deFileResource{
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
	deSound(deSoundManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime, int bytesPerSample, int sampleRate,
		int sampleCount, int channelCount);
	
	/**
	 * \brief Create sound for internal loading.
	 * \warning This is a special internal constructor. Never ever call this on your own!
	 */
	deSound(deSoundManager *manager, deVirtualFileSystem *vfs, const char *filename,
		TIME_SYSTEM modificationTime);
	
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
	void SetPeerAudio(deBaseAudioSound *peer);
	
	/** \brief Synthesizer system peer object. */
	inline deBaseSynthesizerSound *GetPeerSynthesizer() const{ return pPeerSynthesizer; }
	
	/** \brief Set synthesizer system peer object. */
	void SetPeerSynthesizer(deBaseSynthesizerSound *peer);
	/*@}*/
	
	
	
	/** \name Special */
	/*@{*/
	/**
	 * \brief Finalize construction after asynchronous loading.
	 * \warning This is a special internal function. Never ever call this on your own!
	 */
	void FinalizeConstruction(int bytesPerSample, int sampleRate,
		int sampleCount, int channelCount);
	/*@}*/
};

#endif

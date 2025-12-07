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

#ifndef _DEOALAVIDEOPLAYER_H_
#define _DEOALAVIDEOPLAYER_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoAudioDecoder.h>

class deoalAudioThread;
class deVideo;



/**
 * \brief Audio video player.
 */
class deoalAVideoPlayer : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	deVideoAudioDecoder::Ref pDecoder;
	deVideoPlayer::ePlayState pPlayState;
	float pPlayPosition;
	float pPlayFrom;
	float pPlayTo;
	float pPlaySpeed;
	bool pLooping;
	
	int pBytesPerSample;
	int pChannelCount;
	int pSampleRate;
	int pSampleCount;
	
	int pBufferSampleSize;
	bool pSeekNextRead;
	
	unsigned int pUpdateTracker;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalAVideoPlayer> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create microphone. */
	deoalAVideoPlayer(deoalAudioThread &audioThread);
	
protected:
	/** \brief Clean up microphone. */
	virtual ~deoalAVideoPlayer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{return pAudioThread;}
	
	/**
	 * \brief Video changed.
	 * \warning Called during synchronization time by main thread.
	 */
	void SetVideo(deVideo *video);
	
	/** \brief Play state. */
	inline deVideoPlayer::ePlayState GetPlayState() const{return pPlayState;}
	
	/** \brief Set play state. */
	void SetPlayState(deVideoPlayer::ePlayState playState);
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{return pPlaySpeed;}
	
	/** \brief Set to play speed. */
	void SetPlaySpeed(float playSpeed);
	
	/** \brief Play looped. */
	inline bool GetLooping() const{return pLooping;}
	
	/** \brief Set to play looped. */
	void SetLooping(bool looping);
	
	/** \brief Play from position. */
	inline float GetPlayFrom() const{return pPlayFrom;}
	
	/** \brief Play to position. */
	inline float GetPlayTo() const{return pPlayTo;}
	
	/** \brief Set play range. */
	void SetPlayRange(float from, float to);
	
	/** \brief Play position. */
	inline float GetPlayPosition() const{return pPlayPosition;}
	
	/** \brief Set play position. */
	void SetPlayPosition(float position, bool seeking);
	
	
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{return pBytesPerSample;}
	
	/** \brief Channel count. */
	inline int GetChannelCount() const{return pChannelCount;}
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{return pSampleRate;}
	
	/** \brief Sample count. */
	inline int GetSampleCount() const{return pSampleCount;}
	
	
	
	/** \brief Update tracker. */
	inline unsigned int GetUpdateTracker() const{return pUpdateTracker;}
	
	
	
	/**
	 * \brief Read samples.
	 * \param[out] buffer Buffer to store samples in.
	 * \param[in] bufferSize Size of buffer in bytes to store samples in. Has to match format.
	 * \param[in] offset Offset in samples to produce sound at.
	 * \param[in] samples Number of samples to produce.
	 * 
	 * \throws EInvalidParam \em bufferSize does not match format.
	 * \throws EInvalidParam \em buffer is NULL.
	 */
	void ReadSamples(void *buffer, int bufferSize, int offset, int samples);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAdvanceUpdateTracker();
};

#endif

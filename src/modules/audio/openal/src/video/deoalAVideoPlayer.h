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

#ifndef _DEOALAVIDEOPLAYER_H_
#define _DEOALAVIDEOPLAYER_H_

#include <dragengine/deObject.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/resources/video/deVideoAudioDecoderReference.h>

class deoalAudioThread;
class deVideo;



/**
 * \brief Audio video player.
 */
class deoalAVideoPlayer : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	deVideoAudioDecoderReference pDecoder;
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
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create microphone. */
	deoalAVideoPlayer( deoalAudioThread &audioThread );
	
protected:
	/** \brief Clean up microphone. */
	virtual ~deoalAVideoPlayer();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/**
	 * \brief Video changed.
	 * \warning Called during synchronization time by main thread.
	 */
	void SetVideo( deVideo *video );
	
	/** \brief Play state. */
	inline deVideoPlayer::ePlayState GetPlayState() const{ return pPlayState; }
	
	/** \brief Set play state. */
	void SetPlayState( deVideoPlayer::ePlayState playState );
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** \brief Set to play speed. */
	void SetPlaySpeed( float playSpeed );
	
	/** \brief Play looped. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set to play looped. */
	void SetLooping( bool looping );
	
	/** \brief Play from position. */
	inline float GetPlayFrom() const{ return pPlayFrom; }
	
	/** \brief Play to position. */
	inline float GetPlayTo() const{ return pPlayTo; }
	
	/** \brief Set play range. */
	void SetPlayRange( float from, float to );
	
	/** \brief Play position. */
	inline float GetPlayPosition() const{ return pPlayPosition; }
	
	/** \brief Set play position. */
	void SetPlayPosition( float position, bool seeking );
	
	
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Channel count. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Sample count. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	
	
	/** \brief Update tracker. */
	inline unsigned int GetUpdateTracker() const{ return pUpdateTracker; }
	
	
	
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
	void ReadSamples( void *buffer, int bufferSize, int offset, int samples );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAdvanceUpdateTracker();
};

#endif

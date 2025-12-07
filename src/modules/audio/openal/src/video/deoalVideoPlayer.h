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

#ifndef _DEOALVIDEOPLAYER_H_
#define _DEOALVIDEOPLAYER_H_

#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/resources/video/deVideoPlayer.h>
#include <dragengine/systems/modules/audio/deBaseAudioVideoPlayer.h>

class deAudioOpenAL;
class deoalAVideoPlayer;
class deoalSpeaker;

class deVideoPlayer;



/**
 * \brief Sound level meter resource peer.
 */
class deoalVideoPlayer : public deBaseAudioVideoPlayer{
private:
	deAudioOpenAL &pOal;
	deVideoPlayer &pVideoPlayer;
	deoalAVideoPlayer *pAVideoPlayer;
	
	bool pDirtyVideo;
	bool pDirtyParameters;
	bool pDirtyPlayPosition;
	bool pRequiresSeeking;
	
	decPointerSet pSpeakers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create peer. */
	deoalVideoPlayer(deAudioOpenAL &oal, deVideoPlayer &videoPlayer);
	
	/** \brief Clean up peer. */
	virtual ~deoalVideoPlayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief VideoPlayer. */
	inline deVideoPlayer &GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Audio peer. */
	inline deoalAVideoPlayer *GetAVideoPlayer() const{ return pAVideoPlayer; }
	
	
	
	/** \brief Add speaker to track. */
	void AddSpeaker(deoalSpeaker *speaker);
	
	/** \brief Remove speaker to track. */
	void RemoveSpeaker(deoalSpeaker *speaker);
	
	/**
	 * \brief Synchronize.
	 * \warning Called during synchronization time from main thread.
	 */
	void Synchronize();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Sound source changed. */
	virtual void SourceChanged();
	
	/** \brief Looping changed. */
	virtual void LoopingChanged();
	
	/** \brief Play range changed. */
	virtual void PlayRangeChanged();
	
	/** \brief Play speed changed. */
	virtual void PlaySpeedChanged();
	
	/**
	 * \brief Play position changed.
	 * \param[in] seeking Changed due to seeking or by deVideoPlayer::Update().
	 */
	virtual void PlayPositionChanged(bool seeking);
	
	/** \brief Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
};

#endif

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
	deoalVideoPlayer( deAudioOpenAL &oal, deVideoPlayer &videoPlayer );
	
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
	void AddSpeaker( deoalSpeaker *speaker );
	
	/** \brief Remove speaker to track. */
	void RemoveSpeaker( deoalSpeaker *speaker );
	
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
	virtual void PlayPositionChanged( bool seeking );
	
	/** \brief Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
};

#endif

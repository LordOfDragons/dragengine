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

#ifndef _DEOALSPEAKER_H_
#define _DEOALSPEAKER_H_

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/audio/deBaseAudioSpeaker.h>

class deAudioOpenAL;
class deoalASpeaker;
class deoalSound;
class deoalSynthesizerInstance;
class deoalVideoPlayer;
class deoalWorld;
class deoalMicrophone;

class deSpeaker;



/**
 * \brief Speaker peer.
 */
class deoalSpeaker : public deBaseAudioSpeaker{
private:
	deAudioOpenAL &pOal;
	deSpeaker &pSpeaker;
	deoalASpeaker *pASpeaker;
	
	deoalWorld *pParentWorld;
	deoalMicrophone *pParentMicrophone;
	deoalSound *pSound;
	deoalSynthesizerInstance *pSynthesizer;
	deoalVideoPlayer *pVideoPlayer;
	
	unsigned int pSynthesizerUpdateTracker;
	
	bool pDirtySpeaker;
	bool pDirtyGeometry;
	bool pDirtyOctreeNode;
	bool pDirtySource;
	bool pDirtySoundDecoder;
	bool pDirtyVelocity;
	bool pDirtyRange;
	bool pDirtyAttenuation;
	bool pDirtyPlayRange;
	bool pDirtyLayerMask;
	bool pDirtyVideoPlayer;
	bool pDirtyResetStreaming;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	decPointerLinkedList::cListEntry pLLSyncMic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create speaker peer. */
	deoalSpeaker( deAudioOpenAL &oal, deSpeaker &speaker );
	
	/** \brief Clean up speaker peer. */
	virtual ~deoalSpeaker();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Speaker engine resource. */
	inline deSpeaker &GetSpeaker() const{ return pSpeaker; }
	
	/** \brief Parent world or \em NULL. */
	inline deoalWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( deoalWorld *world );
	
	/** \brief Parent microphone. */
	inline deoalMicrophone *GetParentMicrophone() const{ return pParentMicrophone; }
	
	/** \brief Set parent microphone. */
	void SetParentMicrophone( deoalMicrophone *microphone );
	
	/** \brief Audio speaker. */
	inline deoalASpeaker *GetASpeaker() const{ return pASpeaker; }
	
	
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/** \brief Video player requires sync. */
	void VideoPlayerRequiresSync();
	
	/** \brief Dirty reset stream. */
	void DirtyResetStreaming();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Sound source changed. */
	virtual void SourceChanged();
	
	
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Velocity changed. */
	virtual void VelocityChanged();
	
	
	
	/** \brief Muted changed. */
	virtual void MutedChanged();
	
	/** \brief Looping changed. */
	virtual void LoopingChanged();
	
	/** \brief Play position changed. */
	virtual void PlayPositionChanged();
	
	/** \brief Play speed changed. */
	virtual void PlaySpeedChanged();
	
	/** \brief Volume changed. */
	virtual void VolumeChanged();
	
	/** \brief Range changed. */
	virtual void RangeChanged();
	
	/** \brief Roll off factor changed. */
	virtual void RollOffChanged();
	
	/** \brief Distance offset changed. */
	virtual void DistanceOffsetChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	
	
	/** \brief Play state changed. */
	virtual void PlayStateChanged();
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
	
	/** \brief Microphone syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncMic(){ return pLLSyncMic; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncMic() const{ return pLLSyncMic; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pSyncSource();
	
	void pRequiresSync();
};

#endif

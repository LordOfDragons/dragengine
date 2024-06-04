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

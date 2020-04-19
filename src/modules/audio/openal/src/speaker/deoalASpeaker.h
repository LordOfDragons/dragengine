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

#ifndef _DEOALASPEAKER_H_
#define _DEOALASPEAKER_H_

#include "../deoalBasics.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decPointerSet.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/sound/deSoundDecoderReference.h>
#include <dragengine/resources/sound/deSpeaker.h>

class deoalAudioThread;
class deoalAMicrophone;
class deoalASound;
class deoalASynthesizerInstance;
class deoalAVideoPlayer;
class deoalSource;
class deoalAWorld;
class deoalWorldOctree;
class deoalEnvironment;
class deoalSpeaker;



/**
 * \brief Speaker peer.
 */
class deoalASpeaker : public deObject{
private:
	deoalAudioThread &pAudioThread;
	deoalSpeaker *pBackLink;
	
	deoalAWorld *pParentWorld;
	deoalAMicrophone *pParentMicrophone;
	deoalWorldOctree *pOctreeNode;
	
	deoalASound *pSound;
	deoalASynthesizerInstance *pSynthesizer;
	deoalAVideoPlayer *pVideoPlayer;
	unsigned int pSourceUpdateTracker;
	deSoundDecoderReference pSoundDecoder;
	
	deSpeaker::eSpeakerType pSpeakerType;
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pVelocity;
	decLayerMask pLayerMask;
	
	bool pPositionless;
	bool pStreaming;
	bool pEnabled;
	bool pFlag;
	bool pNeedsInitialDecode;
	
	bool pDirtyGeometry;
	bool pDirtyGain;
	bool pDirtyLooping;
	bool pDirtyAttenuation;
	bool pDirtyPlaySpeed;
	bool pDirtyPlayState;
	bool pDirtyPlayPosition;
	
	deSpeaker::ePlayStates pPlayState;
	bool pLooping;
	bool pMuted;
	bool pRestart;
	
	deoalSource *pSource;
	int pBufferSampleCount;
	int pBufferSize;
	int pBufferSampleSize;
	char *pBufferData;
	int pBufferDataCapacity;
	int pQueueSampleOffset;
	ALenum pBufferFormat;
	
	int pSampleRate;
	int pPlayPosition;
	int pPlayFrom;
	int pPlayTo;
	int pPlayRange;
	float pPlaySpeed;
	int pSpeakerPlayFrom;
	int pSpeakerPlayTo;
	
	bool pPlayFinished;
	
	float pRange;
	float pRangeSquared;
	float pVolume;
	float pAttenuationRefDist;
	float pAttenuationRolloff;
	
	float pAttenuatedGain;
	deoalEnvironment *pEnvironment;
	
	decPointerSet pSoundLevelMeters;
	
	bool pMicrophoneMarkedRemove;
	bool pWorldMarkedRemove;
	deoalASpeaker *pLLWorldPrev;
	deoalASpeaker *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create speaker peer. */
	deoalASpeaker( deoalAudioThread &audioThread );
	
protected:
	/** \brief Clean up speaker peer. */
	virtual ~deoalASpeaker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** \brief Back link for use with sound level meter only. */
	inline deoalSpeaker *GetBackLink() const{ return pBackLink; }
	
	/** \brief Back link for use with sound level meter only. */
	void SetBackLink( deoalSpeaker *speaker );
	
	
	
	/** \brief Sound object or \em NULL. */
	inline deoalASound *GetSound() const{ return pSound; }
	
	/** \brief Synthesizer instance object or \em NULL. */
	inline deoalASynthesizerInstance *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Video player object or \em NULL. */
	inline deoalAVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/**
	 * \brief Set source.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetSource( deoalASound *sound, deoalASynthesizerInstance *synthesizer,
		deoalAVideoPlayer *videoPlayer );
	
	/** \brief Sound decoder or NULL. */
	inline deSoundDecoder *GetSoundDecoder() const{ return pSoundDecoder; }
	
	/**
	 * \brief Set sound decoder or NULL.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetSoundDecoder( deSoundDecoder *decoder );
	
	
	
	/** \brief Type. */
	inline deSpeaker::eSpeakerType GetSpeakerType() const{ return pSpeakerType; }
	
	/** \brief Set type. */
	void SetSpeakerType( deSpeaker::eSpeakerType type );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set geometry. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	
	/** \brief Velocity. */
	inline const decVector &GetVelocity() const{ return pVelocity; }
	
	/** \brief Set velocity. */
	void SetVelocity( const decVector &velocity );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Source or \em NULL if not bound. */
	inline deoalSource *GetSource() const{ return pSource; }
	
	/** \brief Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/**
	 * \brief Set if enabled.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetEnabled( bool enabled );
	
	/** \brief Positionless. */
	inline bool GetPositionless() const{ return pPositionless; }
	
	/** \brief Set if positionless. */
	void SetPositionless( bool positionless );
	
	
	
	/** \brief Speaker is playing. */
	inline bool GetPlaying() const{ return pPlayState == deSpeaker::epsPlaying; }
	
	/** \brief Speaker is paused. */
	inline bool GetPaused() const{ return pPlayState == deSpeaker::epsPaused; }
	
	/** \brief Speaker is stopped. */
	inline bool GetStopped() const{ return pPlayState == deSpeaker::epsStopped; }
	
	/** \brief Play state. */
	inline deSpeaker::ePlayStates GetPlayState() const{ return pPlayState; }
	
	/**
	 * \brief Set play state.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetPlayState( deSpeaker::ePlayStates playState );
	
	/** \brief Looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set looping. */
	void SetLooping( bool looping );
	
	/** \brief Muted. */
	inline bool GetMuted() const{ return pMuted; }
	
	/** \brief Set mutex. */
	void SetMuted( bool muted );
	
	/** \brief Set speaker play range. */
	void SetSpeakerPlayRange( int from, int to );
	
	
	
	/** \brief Playback finished. */
	inline bool GetPlayFinished() const{ return pPlayFinished; }
	
	
	
	/** \brief Flag. */
	inline bool GetFlag() const{ return pFlag; }
	
	/** \brief Set flag. */
	void SetFlag( bool flag );
	
	/** \brief Prepare process audio. */
	void PrepareProcessAudio();
	
	/** \brief Process deactivate. */
	void ProcessDeactivate();
	
	/**
	 * \brief Update effects if audible.
	 * 
	 * Called only for speakers in the audible range. If effects are enabled updates them.
	 * Also updates parameters which can not be updated in Update() because they are
	 * affected by effects
	 */
	void UpdateEffects();
	
	/**
	 * \brief Synchronize streaming.
	 * \warning Called during synchronization time from main thread.
	 */
	void SynchronizeStreaming();
	
	/**
	 * \brief Reset streaming.
	 * \warning Called during synchronization time from main thread.
	 */
	void ResetStreaming();
	
	/** \brief Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** \brief Parent world or \em NULL. */
	inline deoalAWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or \em NULL. */
	void SetParentWorld( deoalAWorld *world );
	
	/** \brief Parent microphone or \em NULL. */
	inline deoalAMicrophone *GetParentMicrophone() const{ return pParentMicrophone; }
	
	/** \brief Set parent microphone or \em NULL. */
	void SetParentMicrophone( deoalAMicrophone *microphone );
	
	/** \brief World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set world octree node or NULL. */
	void SetOctreeNode( deoalWorldOctree *node );
	
	/** \brief Update octree node. */
	void UpdateOctreeNode();
	
	
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** \brief Set play speed. */
	void SetPlaySpeed( float speed );
	
	
	
	/** \brief Range. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Range squared. */
	inline float GetRangeSquared() const{ return pRangeSquared; }
	
	/** \brief Set range. */
	void SetRange( float range );
	
	/** \brief Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	/** \brief Attenuation reference distance. */
	inline float GetAttenuationRefDist() const{ return pAttenuationRefDist; }
	
	/** \brief Attenuation rolloff. */
	inline float GetAttenuationRolloff() const{ return pAttenuationRolloff; }
	
	/** \brief Set roll-off. */
	void SetAttenuationRolloff( float rolloff );
	
	/** \brief Calculate gain for distance using attenuation. */
	float AttenuatedGain( float distance ) const;
	
	
	
	/** \brief Environment or \em NULL if not present. */
	inline deoalEnvironment *GetEnvironment() const{ return pEnvironment; }
	
	
	
	/** \brief Sound level meters tracking this speaker. */
	inline decPointerSet &GetSoundLevelMeters(){ return pSoundLevelMeters; }
	inline const decPointerSet &GetSoundLevelMeters() const{ return pSoundLevelMeters; }
	
	
	
	/** \brief Speaker affects active microphone. */
	bool AffectsActiveMicrophone() const;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoalAMicrophone only. Non-thread safe.
	 */
	inline bool GetMicrophoneMarkedRemove() const{ return pMicrophoneMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoalAMicrophone only. Non-thread safe.
	 */
	void SetMicrophoneMarkedRemove( bool marked );
		
	/**
	 * \brief Marked for removal.
	 * \details For use by deoalAWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoalAWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
		
	/** \brief Linked list world previous. */
	inline deoalASpeaker *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev( deoalASpeaker *speaker );
	
	/** \brief Linked list world next. */
	inline deoalASpeaker *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
	void SetLLWorldNext( deoalASpeaker *speaker );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pDecodeInitial();
	void pDecodeNext( bool underrun );
	void pSynthInit();
	void pSynthNext( bool underrun );
	void pVideoPlayerInit();
	void pVideoPlayerNext( bool underrun );
	void pUpdatePlayRange();
	void pUpdateAttenuation();
	
	void pDoPlayState();
	void pStartPlaySource();
	void pStopPlaySource();
	
	void pCheckStillSourceOwner();
	void pUpdateSourceImportance();
	void pUpdateAttenuatedGain();
	
	void pInitSoundSource();
	
	void pEnsureEnvironment();
	void pUpdateEnvironmentEffect();
	
	void pRemoveFromSoundLevelMeters();
	void pDropEnvProbeOctreeNodeAllSLMs();
};

#endif

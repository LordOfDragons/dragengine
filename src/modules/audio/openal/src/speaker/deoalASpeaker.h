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
 * Speaker peer.
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
	float pAttenuationDistanceOffset;
	
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
	/** Create speaker peer. */
	deoalASpeaker( deoalAudioThread &audioThread );
	
protected:
	/** Clean up speaker peer. */
	virtual ~deoalASpeaker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	/** Back link for use with sound level meter only. */
	inline deoalSpeaker *GetBackLink() const{ return pBackLink; }
	
	/** Back link for use with sound level meter only. */
	void SetBackLink( deoalSpeaker *speaker );
	
	
	
	/** Sound object or \em NULL. */
	inline deoalASound *GetSound() const{ return pSound; }
	
	/** Synthesizer instance object or \em NULL. */
	inline deoalASynthesizerInstance *GetSynthesizer() const{ return pSynthesizer; }
	
	/** Video player object or \em NULL. */
	inline deoalAVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/**
	 * Set source.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetSource( deoalASound *sound, deoalASynthesizerInstance *synthesizer,
		deoalAVideoPlayer *videoPlayer );
	
	/** Sound decoder or NULL. */
	inline deSoundDecoder *GetSoundDecoder() const{ return pSoundDecoder; }
	
	/**
	 * Set sound decoder or NULL.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetSoundDecoder( deSoundDecoder *decoder );
	
	
	
	/** Type. */
	inline deSpeaker::eSpeakerType GetSpeakerType() const{ return pSpeakerType; }
	
	/** Set type. */
	void SetSpeakerType( deSpeaker::eSpeakerType type );
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** Set geometry. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	
	/** Velocity. */
	inline const decVector &GetVelocity() const{ return pVelocity; }
	
	/** Set velocity. */
	void SetVelocity( const decVector &velocity );
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** Source or \em NULL if not bound. */
	inline deoalSource *GetSource() const{ return pSource; }
	
	/** Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/**
	 * Set if enabled.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetEnabled( bool enabled );
	
	/** Positionless. */
	inline bool GetPositionless() const{ return pPositionless; }
	
	/** Set if positionless. */
	void SetPositionless( bool positionless );
	
	
	
	/** Speaker is playing. */
	inline bool GetPlaying() const{ return pPlayState == deSpeaker::epsPlaying; }
	
	/** Speaker is paused. */
	inline bool GetPaused() const{ return pPlayState == deSpeaker::epsPaused; }
	
	/** Speaker is stopped. */
	inline bool GetStopped() const{ return pPlayState == deSpeaker::epsStopped; }
	
	/** Play state. */
	inline deSpeaker::ePlayStates GetPlayState() const{ return pPlayState; }
	
	/**
	 * Set play state.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetPlayState( deSpeaker::ePlayStates playState );
	
	/** Looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** Set looping. */
	void SetLooping( bool looping );
	
	/** Muted. */
	inline bool GetMuted() const{ return pMuted; }
	
	/** Set mutex. */
	void SetMuted( bool muted );
	
	/** Set speaker play range. */
	void SetSpeakerPlayRange( int from, int to );
	
	
	
	/** Playback finished. */
	inline bool GetPlayFinished() const{ return pPlayFinished; }
	
	
	
	/** Flag. */
	inline bool GetFlag() const{ return pFlag; }
	
	/** Set flag. */
	void SetFlag( bool flag );
	
	/** Prepare process audio. */
	void PrepareProcessAudio();
	
	/** Process deactivate. */
	void ProcessDeactivate();
	
	/**
	 * Update effects if audible.
	 * 
	 * Called only for speakers in the audible range. If effects are enabled updates them.
	 * Also updates parameters which can not be updated in Update() because they are
	 * affected by effects
	 */
	void UpdateEffects();
	
	/**
	 * Synchronize streaming.
	 * \warning Called during synchronization time from main thread.
	 */
	void SynchronizeStreaming();
	
	/**
	 * Reset streaming.
	 * \warning Called during synchronization time from main thread.
	 */
	void ResetStreaming();
	
	/** Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** Parent world or \em NULL. */
	inline deoalAWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** Set parent world or \em NULL. */
	void SetParentWorld( deoalAWorld *world );
	
	/** Parent microphone or \em NULL. */
	inline deoalAMicrophone *GetParentMicrophone() const{ return pParentMicrophone; }
	
	/** Set parent microphone or \em NULL. */
	void SetParentMicrophone( deoalAMicrophone *microphone );
	
	/** World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** Set world octree node or NULL. */
	void SetOctreeNode( deoalWorldOctree *node );
	
	/** Update octree node. */
	void UpdateOctreeNode();
	
	
	
	/** Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** Set play speed. */
	void SetPlaySpeed( float speed );
	
	
	
	/** Range. */
	inline float GetRange() const{ return pRange; }
	
	/** Range squared. */
	inline float GetRangeSquared() const{ return pRangeSquared; }
	
	/** Set range. */
	void SetRange( float range );
	
	/** Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** Set volume. */
	void SetVolume( float volume );
	
	/** Attenuation reference distance. */
	inline float GetAttenuationRefDist() const{ return pAttenuationRefDist; }
	
	/** Attenuation rolloff. */
	inline float GetAttenuationRolloff() const{ return pAttenuationRolloff; }
	
	/** Set roll-off. */
	void SetAttenuationRolloff( float rolloff );
	
	/** Attenuation distance offset. */
	inline float GetAttenuationDistanceOffset() const{ return pAttenuationDistanceOffset; }
	
	/** Set attenuation distance offset. */
	void SetAttenuationDistanceOffset( float distanceOffset );
	
	/** Calculate gain for distance using attenuation. */
	float AttenuatedGain( float distance ) const;
	
	
	
	/** Environment or \em NULL if not present. */
	inline deoalEnvironment *GetEnvironment() const{ return pEnvironment; }
	
	
	
	/** Sound level meters tracking this speaker. */
	inline decPointerSet &GetSoundLevelMeters(){ return pSoundLevelMeters; }
	inline const decPointerSet &GetSoundLevelMeters() const{ return pSoundLevelMeters; }
	
	
	
	/** Speaker affects active microphone. */
	bool AffectsActiveMicrophone() const;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/**
	 * Marked for removal.
	 * For use by deoalAMicrophone only. Non-thread safe.
	 */
	inline bool GetMicrophoneMarkedRemove() const{ return pMicrophoneMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * For use by deoalAMicrophone only. Non-thread safe.
	 */
	void SetMicrophoneMarkedRemove( bool marked );
		
	/**
	 * Marked for removal.
	 * For use by deoalAWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * For use by deoalAWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
		
	/** Linked list world previous. */
	inline deoalASpeaker *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** Set linked list world previous. */
	void SetLLWorldPrev( deoalASpeaker *speaker );
	
	/** Linked list world next. */
	inline deoalASpeaker *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** Set linked list world next. */
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
	
	bool pUseCustomGain() const;
	float pCustomGainMultiplier() const;
};

#endif

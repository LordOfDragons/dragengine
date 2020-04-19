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

#ifndef _DESPEAKER_H_
#define _DESPEAKER_H_

#include "deSoundReference.h"
#include "../deResource.h"
#include "../synthesizer/deSynthesizerInstanceReference.h"
#include "../video/deVideoPlayerReference.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"
#include "../../common/shape/decShapeList.h"

class deSpeakerManager;
class deWorld;
class deMicrophone;
class deBaseAudioSpeaker;
class deBaseScriptingSpeaker;


/**
 * \brief Speaker resource.
 *
 * Defines a sound or music emmiting source in a world similar to a speaker. Every speaker can
 * play a sound file once or in a loop multiple times as well as music. To avoid a lot of adding
 * and removing of speakers they can be muted individually preventing them from emitting sound
 * or music without removing them. Speakers usually act like a point source emitting sound in
 * all directions. Optionally the speaker can be set to be directed like a spot light emitting
 * sound only in one direction.
 * 
 * The range and roll-off factor is used to shape the attenuation of the speaker. The roll-off
 * factor works similar to the Inverse Distance attenuation model. A value of 1 indicates
 * physically realistic behavior. Values above 1 produce stronger attenuation while values
 * less than 1 produce weaker attenuation. Beyond the range the sound becomes fully inaudible.
 * The sound modules ensure the sound is attenuated in a way it reaches 0 at the range boundary.
 * Typically this is done with a linear scaling across the entire range but can be chosen by
 * the audio module to produce best results.
 * 
 * The best way to set these parameters is to start with the default values to get a physically
 * realistic sound attenuation. Then adjust the range to be smaller to reduce the number of
 * speakers affecting the listener at the same time. Doing so usually saves processing time.
 * If the range becomes small it might be required to alter the roll-off to compensate. Usually
 * though there is no need to change the roll-off value.
 */
class deSpeaker : public deResource{
public:
	/** \brief Speaker type. */
	enum eSpeakerType{
		/** \brief Speaker emits equally in all directions. */
		estPoint,
		
		/** \brief Speaker emits conically along Z axis. */
		estDirected
	};
	
	/** \brief Play states. */
	enum ePlayStates{
		/** \brief Speaker is not playing back. */
		epsStopped,
		
		/** \brief Speaker is playing back. */
		epsPlaying,
		
		/** \brief Playback is paused. */
		epsPaused
	};
	
	
	
private:
	eSpeakerType pType;
	deSoundReference pSound;
	deSynthesizerInstanceReference pSynthesizer;
	deVideoPlayerReference pVideoPlayer;
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pVelocity;
	
	bool pMuted;
	ePlayStates pPlayState;
	bool pLooping;
	int pPlayFrom;
	int pPlayTo;
	float pPlaySpeed;
	float pVolume;
	float pRange;
	float pRollOff;
	decShapeList pShape;
	decLayerMask pLayerMask;
	
	deBaseAudioSpeaker *pPeerAudio;
	deBaseScriptingSpeaker *pPeerScripting;
	
	deWorld *pParentWorld;
	deSpeaker *pLLWorldPrev;
	deSpeaker *pLLWorldNext;
	
	deMicrophone *pParentMicrophone;
	deSpeaker *pLLMicrophonePrev;
	deSpeaker *pLLMicrophoneNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create speaker. */
	deSpeaker( deSpeakerManager *manager );
	
protected:
	/**
	 * \brief Clean up speaker object.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSpeaker();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eSpeakerType GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( eSpeakerType type );
	
	/** \brief Sound or NULL if not set. */
	inline deSound *GetSound() const{ return pSound; }
	
	/** \brief Set sound or NULL if not set. */
	void SetSound( deSound *sound );
	
	/** \brief Synthesizer or NULL if not set. */
	inline deSynthesizerInstance *GetSynthesizer() const{ return pSynthesizer; }
	
	/** \brief Set synthesizer or NULL if not set. */
	void SetSynthesizer( deSynthesizerInstance *synthesizer );
	
	/** \brief Video player or NULL if not set. */
	inline deVideoPlayer *GetVideoPlayer() const{ return pVideoPlayer; }
	
	/** \brief Set video player or NULL if not set. */
	void SetVideoPlayer( deVideoPlayer *videoPlayer );
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Velocity in m/s. */
	inline const decVector &GetVelocity() const{ return pVelocity; }
	
	/** \brief Set velocity in m/s. */
	void SetVelocity( const decVector &velocity );
	
	
	
	/** \brief Speaker is muted. */
	inline bool GetMuted() const{ return pMuted; }
	
	/** \brief Set if speaker is muted. */
	void SetMuted( bool muted );
	
	/** \brief Speaker is playing once or is looping. */
	inline bool GetLooping() const{ return pLooping; }
	
	/** \brief Set if speaker is playing once or is looping. */
	void SetLooping( bool looping );
	
	/** \brief Start play position in samples. */
	inline int GetPlayFrom() const{ return pPlayFrom; }
	
	/** \brief End play position in samples. */
	inline int GetPlayTo() const{ return pPlayTo; }
	
	/** \brief Set play position in samples. */
	void SetPlayPosition( int playFrom, int playTo );
	
	/** \brief Play speed. */
	inline float GetPlaySpeed() const{ return pPlaySpeed; }
	
	/** \brief Set play speed. */
	void SetPlaySpeed( float playSpeed );
	
	/** \brief Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	/** \brief Range beyond which the sound is inaudible. */
	inline float GetRange() const{ return pRange; }
	
	/** \brief Set range beyond which the sound is inaudible. */
	void SetRange( float range );
	
	/**
	 * \brief Roll off factor.
	 * 
	 * 1 is physically realistic. Larger than 1 applies stronger attenuation. Smaller values
	 * apply weaker attenuation.
	 */
	inline float GetRollOff() const{ return pRollOff; }
	
	/**
	 * \brief Set roll off factor.
	 * 
	 * 1 is physically realistic. Larger than 1 applies stronger attenuation. Smaller values
	 * apply weaker attenuation.
	 */
	void SetRollOff( float rollOff );
	
	/** \brief Sound shape. */
	inline const decShapeList &GetShape() const{ return pShape; }
	
	/** \brief Set sound shape. */
	void SetShape( const decShapeList &shape );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Play state. */
	inline ePlayStates GetPlayState() const{ return pPlayState; }
	
	/** \brief Speaker is playing. */
	inline bool GetPlaying() const{ return pPlayState == epsPlaying; }
	
	/** \brief Speaker is paused. */
	inline bool GetPaused() const{ return pPlayState == epsPaused; }
	
	/** \brief Speaker is stopped. */
	inline bool GetStopped() const{ return pPlayState == epsStopped; }
	
	/** \brief Set play state. */
	void SetPlayState( ePlayStates playState );
	
	/** \brief Start playing. */
	void Play();
	
	/** \brief Stop playing. */
	void Stop();
	
	/** \brief Pause playing. */
	void Pause();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Audio system peer or NULL if not set. */
	inline deBaseAudioSpeaker *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer or NULL if not set. */
	void SetPeerAudio( deBaseAudioSpeaker *peer );
	
	/** \brief Scripting system peer. */
	inline deBaseScriptingSpeaker *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer. */
	void SetPeerScripting( deBaseScriptingSpeaker *peer );
	/*@}*/
	
	
	
	/** \name Linked list world */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous speaker in the parent world linked list. */
	inline deSpeaker *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next speaker in the parent world linked list. */
	void SetLLWorldPrev( deSpeaker *speaker );
	
	/** \brief Next speaker in the parent world linked list. */
	inline deSpeaker *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next speaker in the parent world linked list. */
	void SetLLWorldNext( deSpeaker *speaker );
	/*@}*/
	
	
	
	/** \name Linked list microphone */
	/*@{*/
	/** \brief Parent microphone or NULL. */
	inline deMicrophone *GetParentMicrophone() const{ return pParentMicrophone; }
	
	/** \brief Set parent microphone or NULL. */
	void SetParentMicrophone( deMicrophone *microphone );
	
	/** \brief Previous speaker in the parent microphone linked list. */
	inline deSpeaker *GetLLMicrophonePrev() const{ return pLLMicrophonePrev; }
	
	/** \brief Set next speaker in the parent microphone linked list. */
	void SetLLMicrophonePrev( deSpeaker *speaker );
	
	/** \brief Next speaker in the parent microphone linked list. */
	inline deSpeaker *GetLLMicrophoneNext() const{ return pLLMicrophoneNext; }
	
	/** \brief Set next speaker in the parent microphone linked list. */
	void SetLLMicrophoneNext( deSpeaker *speaker );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

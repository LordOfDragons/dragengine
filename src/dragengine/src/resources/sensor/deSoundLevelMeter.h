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

#ifndef _DESOUNDLEVELMETER_H_
#define _DESOUNDLEVELMETER_H_

#include "../deResource.h"
#include "../sound/deSpeaker.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"


class deBaseAudioSoundLevelMeter;
class deBaseScriptingSoundLevelMeter;
class deSoundLevelMeterManager;
class deWorld;


/**
 * \brief Sound level meter Resource.
 *
 * Sound level meters are sensors to measure the sound level present in a specific spot in
 * the game world. The meter also stores a list of all speakers contributing to the sound
 * level. Sound level meters allow adding sound sensing capabilities to game entities.
 * 
 * To use a sound level meter place the resource in the game world and set the desired
 * audible range and directivity. The audio module updates the metering results as soon as
 * possible. This can happen the next frame or a couple of frames later depending on how
 * the audio module operates internally.
 * 
 * Sound level meters are only affected by speakers and components matching the layer mask
 * set in the meter. Furthermore meters can be enabled and disabled to stop measuring while
 * not in use without needing to remove and add the meter from the game all the time.
 */
class DE_DLL_EXPORT deSoundLevelMeter : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deSoundLevelMeter> Ref;
	
	
	
public:
	/** \brief Meter type. */
	enum eMeterTypes {
		/** \brief Meter senses sound all directions. */
		emtPoint,
		
		/** \brief Meter senses sound in a cone along Z axis. */
		emtDirected
	};
	
	
	
	/**
	 * \brief Audible speaker.
	 */
	class DE_DLL_EXPORT cAudibleSpeaker {
	private:
		deSpeaker::Ref pSpeaker;
		float pVolume;
		
	public:
		/** \brief Create audible speaker. */
		cAudibleSpeaker();
		cAudibleSpeaker( deSpeaker *speaker, float volume );
		
		/** \brief Speaker. */
		inline deSpeaker *GetSpeaker() const{ return pSpeaker; }
		
		/** \brief Set speaker. */
		void SetSpeaker( deSpeaker *speaker );
		
		/** \brief Volume. */
		inline float GetVolume() const{ return pVolume; }
		
		/** \brief Set volume. */
		void SetVolume( float volume );
		
		/** \brief Copy audible speaker. */
		cAudibleSpeaker &operator=( const cAudibleSpeaker &speaker );
	};
	
	
	
private:
	   eMeterTypes pType;
	decDVector pPosition;
	decQuaternion pOrientation;
	float pConeAngle;
	decLayerMask pLayerMask;
	float pAudibleDistance;
	bool pEnabled;
	
	deBaseAudioSoundLevelMeter *pPeerAudio;
	deBaseScriptingSoundLevelMeter *pPeerScripting;
	
	deWorld *pParentWorld;
	deSoundLevelMeter *pLLWorldPrev;
	deSoundLevelMeter *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new lumimeter. */
	deSoundLevelMeter( deSoundLevelMeterManager *manager );
	
protected:
	/**
	 * \brief Clean up lumimeter.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deSoundLevelMeter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eMeterTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( eMeterTypes type );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Cone angle in radians measured from cone center to outer hull. */
	inline float GetConeAngle() const{ return pConeAngle; }
	
	/** \brief Set cone angle in radians measured from cone center to outer hull. */
	void SetConeAngle( float angle );
	
	/** \brief Audible distance in meters.*/
	inline float GetAudibleDistance() const{ return pAudibleDistance; }
	
	/** \brief Set audible distance in meters. */
	void SetAudibleDistance( float audibleDistance );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Measuring is enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set if measuring is enabled. */
	void SetEnabled( bool enabled );
	/*@}*/
	
	
	
	/** \name Measuring */
	/*@{*/
	/** \brief Number of audible speakers. */
	int GetAudibleSpeakerCount() const;
	
	/** \brief Audible speaker at index. */
	const cAudibleSpeaker &GetAudibleSpeakerAt( int index ) const;
	
	/**
	 * \brief Notify scripting module speaker became audible.
	 */
	void NotifySpeakerAudible( const cAudibleSpeaker &speaker );
	
	/**
	 * \brief Notify scripting module speaker became inaudible.
	 */
	void NotifySpeakerInaudible( deSpeaker *speaker );
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Audio system peer. */
	inline deBaseAudioSoundLevelMeter *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer. */
	void SetPeerAudio( deBaseAudioSoundLevelMeter *peer );
	
	/** \brief Scripting system peer. */
	inline deBaseScriptingSoundLevelMeter *GetPeerScripting() const{ return pPeerScripting; }
	
	/** \brief Set scripting system peer. */
	void SetPeerScripting( deBaseScriptingSoundLevelMeter *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous lumimeter in the parent world linked list. */
	inline deSoundLevelMeter *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next lumimeter in the parent world linked list. */
	void SetLLWorldPrev( deSoundLevelMeter *lumimeter );
	
	/** \brief Next lumimeter in the parent world linked list. */
	inline deSoundLevelMeter *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next lumimeter in the parent world linked list. */
	void SetLLWorldNext( deSoundLevelMeter *lumimeter );
	/*@}*/
};

#endif

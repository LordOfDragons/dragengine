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

#ifndef _DEMICROPHONE_H_
#define _DEMICROPHONE_H_

#include "../deResource.h"
#include "../../common/math/decMath.h"
#include "../../common/utils/decLayerMask.h"

class deWorld;
class deSpeaker;
class deMicrophone;
class deMicrophoneManager;
class deBaseAudioMicrophone;


/**
 * \brief Microphone Class.
 *
 * Defines a microphone object able to record sounds in a world. The recorded sound is audible
 * at the users speakers afterwards. Only one microphone can be active at any time. A microphone
 * only records sound emitted by speakers in the same world. This way you can switch between
 * microphones without the need for changing a lot of properties each time. Microphones can be
 * of point type recording sound from all directions or directed to record only sound coming
 * from a major direction.
 * 
 * The layer masks affects which speakers the microphone can hear and which components affect
 * sound propagation. Speakers assigned to the microphone are not affected by the layer mask.
 */
class DE_DLL_EXPORT deMicrophone : public deResource{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deMicrophone> Ref;
	
	
	
public:
	enum eMicrophoneType{
		emtPoint,
		emtDirected
	};
	
	
	
private:
	eMicrophoneType pType;
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pVelocity;
	
	bool pMuted;
	float pVolume;
	
	decLayerMask pLayerMask;
	float pSpeakerGain;
	
	deSpeaker *pSpeakerRoot;
	deSpeaker *pSpeakerTail;
	int pSpeakerCount;
	
	deBaseAudioMicrophone *pPeerAudio;
	
	deWorld *pParentWorld;
	deMicrophone *pLLWorldPrev;
	deMicrophone *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create microphone. */
	deMicrophone( deMicrophoneManager *manager );
	
protected:
	/**
	 * \brief Clean up microphone.
	 * \note Subclasses should set their destructor protected too to avoid users
	 * accidently deleting a reference counted object through the object
	 * pointer. Only FreeReference() is allowed to delete the object.
	 */
	virtual ~deMicrophone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Type. */
	inline eMicrophoneType GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( eMicrophoneType type );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Set position. */
	void SetPosition( const decDVector &position );
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set orientation. */
	void SetOrientation( const decQuaternion &orientation );
	
	/** \brief Linear velocity in m/s. */
	inline const decVector &GetVelocity() const{ return pVelocity; }
	
	/** \brief Set velocity in m/s. */
	void SetVelocity( const decVector &velocity );
	
	/** \brief Microphone is muted. */
	inline bool GetMuted() const{ return pMuted; }
	
	/** \brief Set if microphone is muted. */
	void SetMuted( bool muted );
	
	/** \brief Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/**
	 * \brief Gain to multiply all speakers with.
	 * \version 1.16
	 */
	inline float GetSpeakerGain() const{ return pSpeakerGain; }
	
	/**
	 * \brief Set gain to multiply all speakers with.
	 * \version 1.16
	 */
	void SetSpeakerGain( float gain );
	/*@}*/
	
	
	
	/** \name Speakers */
	/*@{*/
	/** \brief Number of speakers. */
	inline int GetSpeakerCount() const{ return pSpeakerCount; }
	
	/** \brief Root speaker or NULL if there are none. */
	inline deSpeaker *GetRootSpeaker() const{ return pSpeakerRoot; }
	
	/**
	 * \brief Add speaker.
	 * \throws deeInvalidParam \em speaker is NULL.
	 * \throws deeInvalidParam \em speaker has a parent world.
	 */
	void AddSpeaker( deSpeaker *speaker );
	
	/**
	 * \brief Remove speaker.
	 * \throws deeInvalidParam \em speaker is NULL.
	 * \throws deeInvalidParam Parent world of \em speaker is not this world.
	 */
	void RemoveSpeaker( deSpeaker *speaker );
	
	/** \brief Remove all speakers. */
	void RemoveAllSpeakers();
	/*@}*/
	
	
	
	/** \name System Peers */
	/*@{*/
	/** \brief Audio system peer. */
	inline deBaseAudioMicrophone *GetPeerAudio() const{ return pPeerAudio; }
	
	/** \brief Set audio system peer. */
	void SetPeerAudio( deBaseAudioMicrophone *peer );
	/*@}*/
	
	
	
	/** \name Linked List */
	/*@{*/
	/** \brief Parent world or NULL. */
	inline deWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deWorld *world );
	
	/** \brief Previous microphone in the parent world linked list. */
	inline deMicrophone *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set next microphone in the parent world linked list. */
	void SetLLWorldPrev( deMicrophone *microphone );
	
	/** \brief Next microphone in the parent world linked list. */
	inline deMicrophone *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set next microphone in the parent world linked list. */
	void SetLLWorldNext( deMicrophone *microphone );
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

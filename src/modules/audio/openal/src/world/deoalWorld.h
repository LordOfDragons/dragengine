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

#ifndef _DEOALWORLD_H_
#define _DEOALWORLD_H_

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/audio/deBaseAudioWorld.h>

class deWorld;
class deAudioOpenAL;
class deoalAWorld;
class deoalComponent;
class deoalSpeaker;
class deoalMicrophone;
class deoalSoundLevelMeter;


/**
 * \brief World Resource Peer.
 */
class deoalWorld : public deBaseAudioWorld{
private:
	deAudioOpenAL &pOal;
	deWorld &pWorld;
	deoalAWorld *pAWorld;
	
	bool pDirtyComponents;
	bool pDirtySpeakers;
	bool pDirtyMicrophones;
	bool pDirtySoundLevelMeters;
	bool pDirtyAllMicLayerMask;
	
	decPointerLinkedList pListSyncComponents;
	decPointerLinkedList pListSyncSpeakers;
	decPointerLinkedList pListSyncMicrophones;
	decPointerLinkedList pListSyncSoundLevelMeters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world peer. */
	deoalWorld( deAudioOpenAL &oal, deWorld &world );
	
	/** \brief Clean up world peer. */
	virtual ~deoalWorld();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief World resource. */
	inline deWorld &GetWorld() const{ return pWorld; }
	
	/** \brief Audio world. */
	inline deoalAWorld *GetAWorld() const{ return pAWorld; }
	
	
	
	/** \brief World is audible. */
	bool IsAudible() const;
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/** \brief Update all microphone layer mask during next synch. */
	void SetDirtyAllMicLayerMask();
	
	/** \brief Developer mode changed. */
	void DevModeChanged();
	
	
	
	/** \brief Add component as require sync if not present. */
	void AddSyncComponent( deoalComponent *component );
	
	/** \brief Remove component as require sync if present. */
	void RemoveSyncComponent( deoalComponent *component );
	
	
	
	/** \brief Add speaker as require sync if not present. */
	void AddSyncSpeaker( deoalSpeaker *speaker );
	
	/** \brief Remove speaker as require sync if present. */
	void RemoveSyncSpeaker( deoalSpeaker *speaker );
	
	
	
	/** \brief Add microphone as require sync if not present. */
	void AddSyncMicrophone( deoalMicrophone *microphone );
	
	/** \brief Remove microphone as require sync if present. */
	void RemoveSyncMicrophone( deoalMicrophone *microphone );
	
	
	
	/** \brief Add sound level meter as require sync if not present. */
	void AddSyncSoundLevelMeter( deoalSoundLevelMeter *soundLevelMeter );
	
	/** \brief Remove sound level meter as require sync if present. */
	void RemoveSyncSoundLevelMeter( deoalSoundLevelMeter *soundLevelMeter );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update world. */
	virtual void Update( float elapsed );
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	
	
	/** \brief Speaker has been added. */
	virtual void SpeakerAdded( deSpeaker *speaker );
	/** \brief Speaker has been removed. */
	
	virtual void SpeakerRemoved( deSpeaker *speaker );
	
	/** \brief All speakers have been removed. */
	virtual void AllSpeakersRemoved();
	
	
	
	/** \brief Microphone has been added. */
	virtual void MicrophoneAdded( deMicrophone *microphone );
	
	/** \brief Microphone has been removed. */
	virtual void MicrophoneRemoved( deMicrophone *microphone );
	
	/** \brief All microphones have been removed. */
	virtual void AllMicrophonesRemoved();
	
	
	
	/** \brief Component has been added. */
	virtual void ComponentAdded( deComponent *component );
	
	/** \brief Component has been removed. */
	virtual void ComponentRemoved( deComponent *component );
	
	/** \brief All components have been removed. */
	virtual void AllComponentsRemoved();
	
	
	
	/** \brief Sound level meter has been added. */
	virtual void SoundLevelMeterAdded( deSoundLevelMeter *soundLevelMeter );
	
	/** \brief Sound level meter has been removed. */
	virtual void SoundLevelMeterRemoved( deSoundLevelMeter *soundLevelMeter );
	
	/** \brief All sound level meters have been removed. */
	virtual void AllSoundLevelMetersRemoved();
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pSyncComponents();
	void pSyncSpeakers();
	void pSyncMicrophones();
	void pSyncSoundLevelMeters();
	
	void pSyncAllMicLayerMask();
};

#endif

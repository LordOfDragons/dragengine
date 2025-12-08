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

#ifndef _DEOALWORLD_H_
#define _DEOALWORLD_H_

#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/audio/deBaseAudioWorld.h>

class deWorld;
class deAudioOpenAL;
#include "deoalAWorld.h"
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
	deoalAWorld::Ref pAWorld;
	
	bool pDirtyComponents;
	bool pDirtySpeakers;
	bool pDirtyMicrophones;
	bool pDirtySoundLevelMeters;
	bool pDirtyAllMicLayerMask;
	bool pDirtyAudioParameters;
	
	decPointerLinkedList pListSyncComponents;
	decPointerLinkedList pListSyncSpeakers;
	decPointerLinkedList pListSyncMicrophones;
	decPointerLinkedList pListSyncSoundLevelMeters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world peer. */
	deoalWorld(deAudioOpenAL &oal, deWorld &world);
	
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
	inline const deoalAWorld::Ref &GetAWorld() const{ return pAWorld; }
	
	
	
	/** \brief World is audible. */
	bool IsAudible() const;
	
	/** \brief Synchronize. */
	void Synchronize();
	
	/** \brief Update all microphone layer mask during next synch. */
	void SetDirtyAllMicLayerMask();
	
	/** \brief Developer mode changed. */
	void DevModeChanged();
	
	
	
	/** \brief Add component as require sync if not present. */
	void AddSyncComponent(deoalComponent *component);
	
	/** \brief Remove component as require sync if present. */
	void RemoveSyncComponent(deoalComponent *component);
	
	
	
	/** \brief Add speaker as require sync if not present. */
	void AddSyncSpeaker(deoalSpeaker *speaker);
	
	/** \brief Remove speaker as require sync if present. */
	void RemoveSyncSpeaker(deoalSpeaker *speaker);
	
	
	
	/** \brief Add microphone as require sync if not present. */
	void AddSyncMicrophone(deoalMicrophone *microphone);
	
	/** \brief Remove microphone as require sync if present. */
	void RemoveSyncMicrophone(deoalMicrophone *microphone);
	
	
	
	/** \brief Add sound level meter as require sync if not present. */
	void AddSyncSoundLevelMeter(deoalSoundLevelMeter *soundLevelMeter);
	
	/** \brief Remove sound level meter as require sync if present. */
	void RemoveSyncSoundLevelMeter(deoalSoundLevelMeter *soundLevelMeter);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Update world. */
	virtual void Update(float elapsed);
	
	/** \brief Size changed. */
	virtual void SizeChanged();
	
	/**
	 * \brief Audio changed.
	 * \version 1.16
	 */
	virtual void AudioChanged();
	
	
	
	/** \brief Speaker has been added. */
	virtual void SpeakerAdded(deSpeaker *speaker);
	/** \brief Speaker has been removed. */
	
	virtual void SpeakerRemoved(deSpeaker *speaker);
	
	/** \brief All speakers have been removed. */
	virtual void AllSpeakersRemoved();
	
	
	
	/** \brief Microphone has been added. */
	virtual void MicrophoneAdded(deMicrophone *microphone);
	
	/** \brief Microphone has been removed. */
	virtual void MicrophoneRemoved(deMicrophone *microphone);
	
	/** \brief All microphones have been removed. */
	virtual void AllMicrophonesRemoved();
	
	
	
	/** \brief Component has been added. */
	virtual void ComponentAdded(deComponent *component);
	
	/** \brief Component has been removed. */
	virtual void ComponentRemoved(deComponent *component);
	
	/** \brief All components have been removed. */
	virtual void AllComponentsRemoved();
	
	
	
	/** \brief Sound level meter has been added. */
	virtual void SoundLevelMeterAdded(deSoundLevelMeter *soundLevelMeter);
	
	/** \brief Sound level meter has been removed. */
	virtual void SoundLevelMeterRemoved(deSoundLevelMeter *soundLevelMeter);
	
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
	void pSyncAudioParameters();
};

#endif

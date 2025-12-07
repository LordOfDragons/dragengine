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

#ifndef _DEOALMICROPHONE_H_
#define _DEOALMICROPHONE_H_

#include "../speaker/deoalSpeakerList.h"

#include <dragengine/common/math/decMath.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/systems/modules/audio/deBaseAudioMicrophone.h>

class deAudioOpenAL;
class deoalAMicrophone;
class deoalSpeaker;
class deoalWorld;

class deMicrophone;



/**
 * \brief Microphone Resource Peer.
 */
class deoalMicrophone : public deBaseAudioMicrophone{
private:
	deAudioOpenAL &pOal;
	const deMicrophone &pMicrophone;
	deoalAMicrophone *pAMicrophone;
	
	deoalWorld *pParentWorld;
	bool pActive;
	
	bool pDirtyMicrophone;
	bool pDirtyGeometry;
	bool pDirtyVelocity;
	bool pDirtySpeakers;
	bool pDirtyOctreeNode;
	bool pDirtyLayerMask;
	
	decPointerLinkedList pListSyncSpeakers;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create microphone. */
	deoalMicrophone(deAudioOpenAL &oal, const deMicrophone &microphone);
	
	/** \brief Clean up microphone. */
	~deoalMicrophone() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{return pOal;}
	
	/** \brief Microphone. */
	inline const deMicrophone &GetMicrophone() const{return pMicrophone;}
	
	/** \brief Audio microphone. */
	inline deoalAMicrophone *GetAMicrophone() const{return pAMicrophone;}
	
	
	
	/** \brief Microphone is active. */
	inline bool GetActive() const{return pActive;}
	
	/** \brief Set if mocrophone is active. */
	void SetActive(bool active);
	
	
	
	/** \brief Parent world or NULL. */
	inline deoalWorld *GetParentWorld() const{return pParentWorld;}
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deoalWorld *world);
	
	
	
	/**
	 * \brief Synchronize.
	 * \warning Called during synchronization time from main thread.
	 */
	void Synchronize();
	
	
	
	/** \brief Add speaker as require sync if not present. */
	void AddSyncSpeaker(deoalSpeaker *speaker);
	
	/** \brief Remove speaker as require sync if present. */
	void RemoveSyncSpeaker(deoalSpeaker *speaker);
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	void TypeChanged() override;
	
	/** \brief Position changed. */
	void PositionChanged() override;
	
	/** \brief Orientation changed. */
	void OrientationChanged() override;
	
	/** \brief Velocity changed. */
	void VelocityChanged() override;
	
	/** \brief Muted changed. */
	void MutedChanged() override;
	
	/** \brief Volume changed. */
	void VolumeChanged() override;
	
	/** \brief Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** \brief Speaker gain changed. */
	void SpeakerGainChanged() override;
	
	/** \brief Enable auralization changed. */
	void EnableAuralizationChanged() override;
	
	/** \brief Speaker has been added. */
	void SpeakerAdded(deSpeaker *speaker) override;
	
	/** \brief Speaker has been removed. */
	void SpeakerRemoved(deSpeaker *speaker) override;
	
	/** \brief All speakers have been removed. */
	void AllSpeakersRemoved() override;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){return pLLSyncWorld;}
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{return pLLSyncWorld;}
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	/** \warning Called during synchronization time from main thread. */
	void pSyncSpeakers();
	
	void pRequiresSync();
};

#endif

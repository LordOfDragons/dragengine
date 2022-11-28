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
	deoalMicrophone( deAudioOpenAL &oal, const deMicrophone &microphone );
	
	/** \brief Clean up microphone. */
	virtual ~deoalMicrophone();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief Microphone. */
	inline const deMicrophone &GetMicrophone() const{ return pMicrophone; }
	
	/** \brief Audio microphone. */
	inline deoalAMicrophone *GetAMicrophone() const{ return pAMicrophone; }
	
	
	
	/** \brief Microphone is active. */
	inline bool GetActive() const{ return pActive; }
	
	/** \brief Set if mocrophone is active. */
	void SetActive( bool active );
	
	
	
	/** \brief Parent world or NULL. */
	inline deoalWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deoalWorld *world );
	
	
	
	/**
	 * \brief Synchronize.
	 * \warning Called during synchronization time from main thread.
	 */
	void Synchronize();
	
	
	
	/** \brief Add speaker as require sync if not present. */
	void AddSyncSpeaker( deoalSpeaker *speaker );
	
	/** \brief Remove speaker as require sync if present. */
	void RemoveSyncSpeaker( deoalSpeaker *speaker );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Velocity changed. */
	virtual void VelocityChanged();
	
	/** \brief Muted changed. */
	virtual void MutedChanged();
	
	/** \brief Volume changed. */
	virtual void VolumeChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Speaker gain changed. */
	virtual void SpeakerGainChanged();
	
	/** \brief Speaker has been added. */
	virtual void SpeakerAdded( deSpeaker *speaker );
	
	/** \brief Speaker has been removed. */
	virtual void SpeakerRemoved( deSpeaker *speaker );
	
	/** \brief All speakers have been removed. */
	virtual void AllSpeakersRemoved();
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	/** \warning Called during synchronization time from main thread. */
	void pSyncSpeakers();
	
	void pRequiresSync();
};

#endif

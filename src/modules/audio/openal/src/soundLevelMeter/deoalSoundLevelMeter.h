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

#ifndef _DEOALSOUNDLEVELMETER_H_
#define _DEOALSOUNDLEVELMETER_H_

#include "../speaker/deoalSpeakerList.h"

#include <dragengine/common/collection/decObjectSet.h>
#include <dragengine/common/collection/decPointerLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/systems/modules/audio/deBaseAudioSoundLevelMeter.h>

class deAudioOpenAL;
class deoalASoundLevelMeter;
class deoalWorld;

class deSoundLevelMeter;



/**
 * \brief Sound level meter resource peer.
 */
class deoalSoundLevelMeter : public deBaseAudioSoundLevelMeter{
private:
	deAudioOpenAL &pOal;
	deSoundLevelMeter &pSoundLevelMeter;
	deoalASoundLevelMeter *pASoundLevelMeter;
	
	deoalWorld *pParentWorld;
	
	bool pDirtyGeometry;
	bool pDirtyOctreeNode;
	bool pDirtyLayerMask;
	bool pDirtyEnabled;
	
	deSoundLevelMeter::cAudibleSpeaker *pSpeakers;
	int pSpeakerCount;
	int pSpeakerSize;
	
	decObjectSet pOldSpeakers;
	
	decPointerLinkedList::cListEntry pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound level meter. */
	deoalSoundLevelMeter( deAudioOpenAL &oal, deSoundLevelMeter &soundLevelMeter );
	
	/** \brief Clean up sound level meter. */
	virtual ~deoalSoundLevelMeter();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief SoundLevelMeter. */
	inline deSoundLevelMeter &GetSoundLevelMeter() const{ return pSoundLevelMeter; }
	
	/** \brief Audio sound level meter. */
	inline deoalASoundLevelMeter *GetASoundLevelMeter() const{ return pASoundLevelMeter; }
	
	
	
	/** \brief Parent world or NULL. */
	inline deoalWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld( deoalWorld *world );
	
	
	
	/** \brief Number of speakers. */
	inline int GetSpeakerCount() const{ return pSpeakerCount; }
	
	/** \brief Speaker at index. */
	const deSoundLevelMeter::cAudibleSpeaker &GetSpeakerAt( int index ) const;
	
	/** \brief Add speaker. */
	void AddSpeaker( deSpeaker *speaker, float volume );
	
	/** \brief Remove all speakers. */
	void RemoveAllSpeakers();
	
	
	
	/**
	 * \brief Synchronize.
	 * \warning Called during synchronization time from main thread.
	 */
	void Synchronize();
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Type changed. */
	virtual void TypeChanged();
	
	/** \brief Position changed. */
	virtual void PositionChanged();
	
	/** \brief Orientation changed. */
	virtual void OrientationChanged();
	
	/** \brief Cone angle changed. */
	virtual void ConeAngleChanged();
	
	/** \brief Audible distance changed. */
	virtual void AudibleDistanceChanged();
	
	/** \brief Layer mask changed. */
	virtual void LayerMaskChanged();
	
	/** \brief Enabled changed. */
	virtual void EnabledChanged();
	
	/** \brief Number of audible speakers. */
	virtual int GetAudibleSpeakerCount();
	
	/** \brief Audible speaker at index. */
	virtual const deSoundLevelMeter::cAudibleSpeaker &GetAudibleSpeakerAt( int index );
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
	inline decPointerLinkedList::cListEntry &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decPointerLinkedList::cListEntry &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pRequiresSync();
	void pUpdateAudibleSpeakers();
	int pIndexOfSpeaker( deSpeaker *speaker ) const;
};

#endif

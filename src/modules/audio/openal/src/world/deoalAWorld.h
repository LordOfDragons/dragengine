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

#ifndef _DEOALAWORLD_H_
#define _DEOALAWORLD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAudioThread;
class deoalAComponent;
class deoalAMicrophone;
class deoalASpeaker;
class deoalASoundLevelMeter;
class deoalWorldOctree;
class deoalWorldOctreeVisitor;


/**
 * \brief Audio thread world.
 */
class deoalAWorld : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	deoalAComponent *pRootComponent;
	deoalAComponent *pTailComponent;
	int pComponentCount;
	
	deoalASpeaker *pRootSpeaker;
	deoalASpeaker *pTailSpeaker;
	int pSpeakerCount;
	
	deoalAMicrophone *pRootMicrophone;
	deoalAMicrophone *pTailMicrophone;
	int pMicrophoneCount;
	
	deoalASoundLevelMeter *pRootSoundLevelMeter;
	deoalASoundLevelMeter *pTailSoundLevelMeter;
	int pSoundLevelMeterCount;
	
	deoalWorldOctree *pOctree;
	
	decLayerMask pAllMicLayerMask;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world peer. */
	deoalAWorld( deoalAudioThread &audioThread, const decDVector &size );
	
protected:
	/** \brief Clean up world peer. */
	virtual ~deoalAWorld();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** \brief Octree. */
	inline deoalWorldOctree *GetOctree() const{ return pOctree; }
	
	/** \brief Visit content of world. */
	void VisitRegion( const decDVector &center, const decDVector &halfSize,
		deoalWorldOctreeVisitor &visitor );
	
	/** \brief Layer mask containing all bits of all microphones. */
	inline const decLayerMask &GetAllMicLayerMask() const{ return pAllMicLayerMask; }
	
	
	
	/** \brief Invalidate environment probes. */
	void InvalidateEnvProbes( const decDVector &minExtend, const decDVector &maxExtend,
		const decLayerMask &layerMask );
	
	
	
	/** \brief World is audible. */
	bool IsAudible() const;
	
	/**
	 * \brief Enable or disable all speakers in the world.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetAllSpeakersEnabled( bool enabled );
	
	/**
	 * \brief Set all microphone layer mask.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetAllMicLayerMask( const decLayerMask &layerMask );
	
	/** \brief Update all speakers. */
	void UpdateAllSpeakers();
	
	/** \brief Update sound level metering. */
	void UpdateSoundLevelMetering();
	
	/** \brief Prepare processing audio for one frame update. */
	void PrepareProcessAudio();
	
	/** \brief Process deactivate. */
	void ProcessDeactivate();
	/*@}*/
	
	
	
	/** \name Components. */
	/*@{*/
	/** \brief Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** \brief Root component. */
	inline deoalAComponent *GetRootComponent() const{ return pRootComponent; }
	
	/** \brief Add component. */
	void AddComponent( deoalAComponent *component );
	
	/** \brief Remove component. */
	void RemoveComponent( deoalAComponent *component );
	
	/** \brief Remove all component. */
	void RemoveAllComponents();
	
	/** \brief Remove components marked for removal. */
	void RemoveRemovalMarkedComponents();
	/*@}*/
	
	
	
	/** \name Speakers. */
	/*@{*/
	/** \brief Number of speakers. */
	inline int GetSpeakerCount() const{ return pSpeakerCount; }
	
	/** \brief Root speaker. */
	inline deoalASpeaker *GetRootSpeaker() const{ return pRootSpeaker; }
	
	/**
	 * \brief Add speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void AddSpeaker( deoalASpeaker *speaker );
	
	/**
	 * \brief Remove speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveSpeaker( deoalASpeaker *speaker );
	
	/**
	 * \brief Remove all speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveAllSpeakers();
	
	/**
	 * \brief Remove speakers marked for removal.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveRemovalMarkedSpeakers();
	/*@}*/
	
	
	
	/** \name Microphones. */
	/*@{*/
	/** \brief Number of microphones. */
	inline int GetMicrophoneCount() const{ return pMicrophoneCount; }
	
	/** \brief Root microphone. */
	inline deoalAMicrophone *GetRootMicrophone() const{ return pRootMicrophone; }
	
	/** \brief Add microphone. */
	void AddMicrophone( deoalAMicrophone *microphone );
	
	/** \brief Remove microphone. */
	void RemoveMicrophone( deoalAMicrophone *microphone );
	
	/** \brief Remove all microphone. */
	void RemoveAllMicrophones();
	
	/** \brief Remove microphones marked for removal. */
	void RemoveRemovalMarkedMicrophones();
	/*@}*/
	
	
	
	/** \name Sound level meters. */
	/*@{*/
	/** \brief Number of sound level meters. */
	inline int GetSoundLevelMeterCount() const{ return pSoundLevelMeterCount; }
	
	/** \brief Root sound level meter. */
	inline deoalASoundLevelMeter *GetRootSoundLevelMeter() const{ return pRootSoundLevelMeter; }
	
	/**
	 * \brief Add sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void AddSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	
	/**
	 * \brief Remove sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	
	/**
	 * \brief Remove all sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveAllSoundLevelMeters();
	
	/**
	 * \brief Remove sound level meters marked for removal.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveRemovalMarkedSoundLevelMeters();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

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
 * Audio thread world.
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
	float pSpeakerGain;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world peer. */
	deoalAWorld( deoalAudioThread &audioThread, const decDVector &size );
	
protected:
	/** Clean up world peer. */
	virtual ~deoalAWorld();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** OpenAL module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** Octree. */
	inline deoalWorldOctree *GetOctree() const{ return pOctree; }
	
	/** Visit content of world. */
	void VisitRegion( const decDVector &center, const decDVector &halfSize,
		deoalWorldOctreeVisitor &visitor );
	
	/** Layer mask containing all bits of all microphones. */
	inline const decLayerMask &GetAllMicLayerMask() const{ return pAllMicLayerMask; }
	
	/** Speaker gain. */
	inline float GetSpeakerGain() const{ return pSpeakerGain; }
	
	/** Set speaker gain. */
	void SetSpeakerGain( float gain );
	
	
	
	/** Invalidate environment probes. */
	void InvalidateEnvProbes( const decDVector &minExtend, const decDVector &maxExtend,
		const decLayerMask &layerMask );
	
	
	
	/** World is audible. */
	bool IsAudible() const;
	
	/**
	 * Enable or disable all speakers in the world.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetAllSpeakersEnabled( bool enabled );
	
	/**
	 * Set all microphone layer mask.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetAllMicLayerMask( const decLayerMask &layerMask );
	
	/** Update all speakers. */
	void UpdateAllSpeakers();
	
	/** Update sound level metering. */
	void UpdateSoundLevelMetering();
	
	/** Prepare processing audio for one frame update. */
	void PrepareProcessAudio();
	
	/** Process deactivate. */
	void ProcessDeactivate();
	/*@}*/
	
	
	
	/** \name Components. */
	/*@{*/
	/** Number of components. */
	inline int GetComponentCount() const{ return pComponentCount; }
	
	/** Root component. */
	inline deoalAComponent *GetRootComponent() const{ return pRootComponent; }
	
	/** Add component. */
	void AddComponent( deoalAComponent *component );
	
	/** Remove component. */
	void RemoveComponent( deoalAComponent *component );
	
	/** Remove all component. */
	void RemoveAllComponents();
	
	/** Remove components marked for removal. */
	void RemoveRemovalMarkedComponents();
	/*@}*/
	
	
	
	/** \name Speakers. */
	/*@{*/
	/** Number of speakers. */
	inline int GetSpeakerCount() const{ return pSpeakerCount; }
	
	/** Root speaker. */
	inline deoalASpeaker *GetRootSpeaker() const{ return pRootSpeaker; }
	
	/**
	 * Add speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void AddSpeaker( deoalASpeaker *speaker );
	
	/**
	 * Remove speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveSpeaker( deoalASpeaker *speaker );
	
	/**
	 * Remove all speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveAllSpeakers();
	
	/**
	 * Remove speakers marked for removal.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveRemovalMarkedSpeakers();
	/*@}*/
	
	
	
	/** \name Microphones. */
	/*@{*/
	/** Number of microphones. */
	inline int GetMicrophoneCount() const{ return pMicrophoneCount; }
	
	/** Root microphone. */
	inline deoalAMicrophone *GetRootMicrophone() const{ return pRootMicrophone; }
	
	/** Add microphone. */
	void AddMicrophone( deoalAMicrophone *microphone );
	
	/** Remove microphone. */
	void RemoveMicrophone( deoalAMicrophone *microphone );
	
	/** Remove all microphone. */
	void RemoveAllMicrophones();
	
	/** Remove microphones marked for removal. */
	void RemoveRemovalMarkedMicrophones();
	/*@}*/
	
	
	
	/** \name Sound level meters. */
	/*@{*/
	/** Number of sound level meters. */
	inline int GetSoundLevelMeterCount() const{ return pSoundLevelMeterCount; }
	
	/** Root sound level meter. */
	inline deoalASoundLevelMeter *GetRootSoundLevelMeter() const{ return pRootSoundLevelMeter; }
	
	/**
	 * Add sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void AddSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	
	/**
	 * Remove sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	
	/**
	 * Remove all sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveAllSoundLevelMeters();
	
	/**
	 * Remove sound level meters marked for removal.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveRemovalMarkedSoundLevelMeters();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

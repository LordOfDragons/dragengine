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

#ifndef _DEOALAMICROPHONE_H_
#define _DEOALAMICROPHONE_H_

#include "../speaker/deoalSpeakerList.h"
#include "../world/octree/deoalWOVPrepareRayTrace.h"
#include "../world/octree/deoalRTWorldBVH.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAudioThread;
class deoalAWorld;
class deoalEnvProbe;
class deoalWorldOctree;
class deoalSoundRay;
class deoalSoundRayList;
class deoalEnvProbeList;

class deDebugBlockInfo;
class deDebugDrawer;
class deDebugDrawerShape;


#define LISTENER_CENTRIC_RAY_CAST 1


/**
 * Audio microphone.
 */
class deoalAMicrophone : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	decDVector pPosition;
	decQuaternion pOrientation;
	decVector pVelocity;
	float pVolume;
	bool pMuted;
	decLayerMask pLayerMask;
	float pSpeakerGain;
	
	decObjectList pSpeakers;
	deoalSpeakerList pActiveSpeakers;
	
	deoalAWorld *pParentWorld;
	deoalWorldOctree *pOctreeNode;
	deoalEnvProbeList *pEnvProbeList;
	bool pActive;
	
	deoalEnvProbe *pEnvProbe;
	bool pDirtyEnvProbe;
	
	bool pDirtyGeometry;
	bool pDirtyGain;
	
	deoalWOVPrepareRayTrace pWOVPrepareRayTrace;
	deoalRTWorldBVH pRTWorldBVH;
	
	bool pWorldMarkedRemove;
	deoalAMicrophone *pLLWorldPrev;
	deoalAMicrophone *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create microphone. */
	deoalAMicrophone( deoalAudioThread &audioThread );
	
protected:
	/** Clean up microphone. */
	virtual ~deoalAMicrophone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** Set geometry. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	
	/** Velocity. */
	inline const decVector &GetVelocity() const{ return pVelocity; }
	
	/** Set velocity. */
	void SetVelocity( const decVector &velocity );
	
	/** Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** Set volume. */
	void SetVolume( float volume );
	
	/** Muted. */
	inline bool GetMuted() const{ return pMuted; }
	
	/** Set muted. */
	void SetMuted( bool mnuted );
	
	/** Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** Gain to multiply all speakers with. */
	inline float GetSpeakerGain() const{ return pSpeakerGain; }
	
	/** Set gain to multiply all speakers with. */
	void SetSpeakerGain( float gain );
	
	
	
	/** Microphone is active. */
	inline bool GetActive() const{ return pActive; }
	
	/**
	 * Set if mocrophone is active.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetActive( bool active );
	
	
	
	/** Number of speakers. */
	int GetSpeakerCount() const;
	
	/** Speaker at index. */
	deoalASpeaker *GetSpeakerAt( int index ) const;
	
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
	 * Remove all speakers.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveAllSpeakers();
	
	/**
	 * Remove speakers marked for removal.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveRemovalMarkedSpeakers();
	
	
	
	/** Active speaker list. */
	inline deoalSpeakerList &GetActiveSpeakers(){ return pActiveSpeakers; }
	inline const deoalSpeakerList &GetActiveSpeakers() const{ return pActiveSpeakers; }
	
	/**
	 * Find active speakers.
	 * 
	 * Finds all speakers inside speaker range. Disables speakers leaving the speaker range
	 * and enables speakers entering the speaker range.
	 * 
	 * \warning Called during synchronization time from main thread.
	 */
	void FindActiveSpeakers();
	
	
	
	/** Parent world or NULL. */
	inline deoalAWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * Set parent world or NULL.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetParentWorld( deoalAWorld *world );
	
	/** World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** Set world octree node or NULL. */
	void SetOctreeNode( deoalWorldOctree *node );
	
	/** Update octree node. */
	void UpdateOctreeNode();
	
	/** Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** Environment probe or NULL if not present. */
	deoalEnvProbe *GetEnvProbe();
	
	/** Ray-trace world bvh. */
	inline deoalRTWorldBVH &GetRTWorldBVH(){ return pRTWorldBVH; }
	
	
	
	/** Process audio. */
	void ProcessAudio();
	
	/** Process deactivate. */
	void ProcessDeactivate();
	
	/** Invalidates speaker. */
	void InvalidateSpeaker( deoalASpeaker *speaker );
	
	/**
	 * Update debug information.
	 * \warning Called during synchronization time from main thread.
	 */
	void DebugUpdateInfo( deDebugBlockInfo &debugInfo );
	void DebugCaptureRays( deDebugDrawer &debugDrawer, bool xray );
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
		
	/** Linked list world previous. */
	inline deoalAMicrophone *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** Set linked list world previous. */
	void SetLLWorldPrev( deoalAMicrophone *microphone );
	
	/** Linked list world next. */
	inline deoalAMicrophone *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** Set linked list world next. */
	void SetLLWorldNext( deoalAMicrophone *microphone );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	/** \warning Called during synchronization time from main thread. */
	void pEnableAttachedSpeakers( bool enable );
	
	void pProcessEffects();
	
	void pDebugCaptureRays( deDebugDrawer &debugDrawer, bool xray );
	void pDebugCaptureRays( deDebugDrawerShape &shape,
		const deoalSoundRayList &rayList, const deoalSoundRay &ray );
	
	float pMaxActiveSpeakerRange() const;
};

#endif

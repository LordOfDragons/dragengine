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
 * \brief Audio microphone.
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
	/** \brief Create microphone. */
	deoalAMicrophone( deoalAudioThread &audioThread );
	
protected:
	/** \brief Clean up microphone. */
	virtual ~deoalAMicrophone();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set geometry. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	
	/** \brief Velocity. */
	inline const decVector &GetVelocity() const{ return pVelocity; }
	
	/** \brief Set velocity. */
	void SetVelocity( const decVector &velocity );
	
	/** \brief Volume. */
	inline float GetVolume() const{ return pVolume; }
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	/** \brief Muted. */
	inline bool GetMuted() const{ return pMuted; }
	
	/** \brief Set muted. */
	void SetMuted( bool mnuted );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	
	
	/** \brief Microphone is active. */
	inline bool GetActive() const{ return pActive; }
	
	/**
	 * \brief Set if mocrophone is active.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetActive( bool active );
	
	
	
	/** \brief Number of speakers. */
	int GetSpeakerCount() const;
	
	/** \brief Speaker at index. */
	deoalASpeaker *GetSpeakerAt( int index ) const;
	
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
	 * \brief Remove all speakers.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveAllSpeakers();
	
	/**
	 * \brief Remove speakers marked for removal.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveRemovalMarkedSpeakers();
	
	
	
	/** \brief Active speaker list. */
	inline deoalSpeakerList &GetActiveSpeakers(){ return pActiveSpeakers; }
	inline const deoalSpeakerList &GetActiveSpeakers() const{ return pActiveSpeakers; }
	
	/**
	 * \brief Find active speakers.
	 * 
	 * Finds all speakers inside speaker range. Disables speakers leaving the speaker range
	 * and enables speakers entering the speaker range.
	 * 
	 * \warning Called during synchronization time from main thread.
	 */
	void FindActiveSpeakers();
	
	
	
	/** \brief Parent world or NULL. */
	inline deoalAWorld *GetParentWorld() const{ return pParentWorld; }
	
	/**
	 * \brief Set parent world or NULL.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetParentWorld( deoalAWorld *world );
	
	/** \brief World octree node or NULL. */
	inline deoalWorldOctree *GetOctreeNode() const{ return pOctreeNode; }
	
	/** \brief Set world octree node or NULL. */
	void SetOctreeNode( deoalWorldOctree *node );
	
	/** \brief Update octree node. */
	void UpdateOctreeNode();
	
	/** \brief Quick dispose. */
	void PrepareQuickDispose();
	
	
	
	/** \brief Environment probe or NULL if not present. */
	deoalEnvProbe *GetEnvProbe();
	
	/** \brief Ray-trace world bvh. */
	inline deoalRTWorldBVH &GetRTWorldBVH(){ return pRTWorldBVH; }
	
	
	
	/** \brief Process audio. */
	void ProcessAudio();
	
	/** \brief Process deactivate. */
	void ProcessDeactivate();
	
	/** \brief Invalidates speaker. */
	void InvalidateSpeaker( deoalASpeaker *speaker );
	
	/**
	 * \brief Update debug information.
	 * \warning Called during synchronization time from main thread.
	 */
	void DebugUpdateInfo( deDebugBlockInfo &debugInfo );
	void DebugCaptureRays( deDebugDrawer &debugDrawer, bool xray );
	/*@}*/
	
	
	
	/** \name Render world usage */
	/*@{*/
	/**
	 * \brief Marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	inline bool GetWorldMarkedRemove() const{ return pWorldMarkedRemove; }
	
	/**
	 * \brief Set marked for removal.
	 * \details For use by deoglRWorld only. Non-thread safe.
	 */
	void SetWorldMarkedRemove( bool marked );
		
	/** \brief Linked list world previous. */
	inline deoalAMicrophone *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev( deoalAMicrophone *microphone );
	
	/** \brief Linked list world next. */
	inline deoalAMicrophone *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
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

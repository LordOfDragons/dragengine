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

#ifndef _DEOALASOUNDLEVELMETER_H_
#define _DEOALASOUNDLEVELMETER_H_

#include "../speaker/deoalSpeakerList.h"
#include "../world/octree/deoalWOVPrepareRayTrace.h"

#include <dragengine/deObject.h>
#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>

class deoalAudioThread;
class deoalAWorld;
class deoalEnvProbe;
class deoalWorldOctree;
class deoalEnvProbeList;
class deoalASoundLevelMeterSpeaker;
class deoalASpeaker;


/**
 * \brief Audio microphone.
 */
class deoalASoundLevelMeter : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	deSoundLevelMeter::eMeterTypes pType;
	decDVector pPosition;
	decQuaternion pOrientation;
	float pConeAngle;
	decLayerMask pLayerMask;
	float pAudibleDistance;
	bool pEnabled;
	
	decObjectList pSpeakers;
	
	deoalAWorld *pParentWorld;
	deoalWorldOctree *pOctreeNode;
	deoalEnvProbeList *pEnvProbeList;
	
	deoalEnvProbe *pEnvProbe;
	bool pDirtyEnvProbe;
	
	deoalWOVPrepareRayTrace pWOVPrepareRayTrace;
	
	bool pWorldMarkedRemove;
	deoalASoundLevelMeter *pLLWorldPrev;
	deoalASoundLevelMeter *pLLWorldNext;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create microphone. */
	deoalASoundLevelMeter( deoalAudioThread &audioThread );
	
protected:
	/** \brief Clean up microphone. */
	virtual ~deoalASoundLevelMeter();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deoalAudioThread &GetAudioThread() const{ return pAudioThread; }
	
	
	
	/** \brief Type. */
	inline deSoundLevelMeter::eMeterTypes GetType() const{ return pType; }
	
	/** \brief Set type. */
	void SetType( deSoundLevelMeter::eMeterTypes type );
	
	/** \brief Position. */
	inline const decDVector &GetPosition() const{ return pPosition; }
	
	/** \brief Orientation. */
	inline const decQuaternion &GetOrientation() const{ return pOrientation; }
	
	/** \brief Set geometry. */
	void SetGeometry( const decDVector &position, const decQuaternion &orientation );
	
	/** \brief Cone angle. */
	inline float GetConeAngle() const{ return pConeAngle; }
	
	/** \brief Set cone angle. */
	void SetConeAngle( float angle );
	
	/** \brief Audible distance. */
	inline float GetAudibleDistance() const{ return pAudibleDistance; }
	
	/** \brief Set audible distance. */
	void SetAudibleDistance( float audibleDistance );
	
	/** \brief Layer mask. */
	inline const decLayerMask &GetLayerMask() const{ return pLayerMask; }
	
	/** \brief Set layer mask. */
	void SetLayerMask( const decLayerMask &layerMask );
	
	/** \brief Enabled. */
	inline bool GetEnabled() const{ return pEnabled; }
	
	/** \brief Set enabled. */
	void SetEnabled( bool enabled );
	
	
	
	/** \brief Number of speakers. */
	int GetSpeakerCount() const;
	
	/** \brief Speaker at index. */
	const deoalASoundLevelMeterSpeaker &GetSpeakerAt( int index ) const;
	
	/** \brief Remove speaker if present. */
	void RemoveSpeakerIfPresent( deoalASpeaker *speaker );
	
	/** \brief Speaker for speaker instance. */
	deoalASoundLevelMeterSpeaker *GetSpeakerWith( deoalASpeaker *speaker ) const;
	
	/** \brief Invalidate speaker env probe. */
	void SpeakerDropEnvProbeOctreeNode( deoalASpeaker *speaker );
	
	
	
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
	
	
	
	/**
	 * \brief Update speakers overlapping sound level meter.
	 * 
	 * Overlapping speakers are not necessarily audible. This list just keeps track of them
	 * so UpdateMeasurements() can gradually update them.
	 */
	void FindSpeakers();
	
	/** \brief Meter speakers. */
	void MeterSpeakers();
	/*@}*/
	
	
	
	/** \name World usage */
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
	inline deoalASoundLevelMeter *GetLLWorldPrev() const{ return pLLWorldPrev; }
	
	/** \brief Set linked list world previous. */
	void SetLLWorldPrev( deoalASoundLevelMeter *soundLevelMeter );
	
	/** \brief Linked list world next. */
	inline deoalASoundLevelMeter *GetLLWorldNext() const{ return pLLWorldNext; }
	
	/** \brief Set linked list world next. */
	void SetLLWorldNext( deoalASoundLevelMeter *soundLevelMeter );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	void pAddSpeaker( deoalASpeaker *speaker );
	int pIndexOfSpeaker( deoalASpeaker *speaker ) const;
	void pRemoveSpeakerFrom( int index );
	void pRemoveAllSpeakers();
	void pFlagAllSpeakers( bool flag ) const;
	void pDropEnvProbeOctreeNodeAllSpeakers();
};

#endif

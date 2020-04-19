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

#ifndef _DEOALWORLDOCTREE_H_
#define _DEOALWORLDOCTREE_H_

#include "../../utils/octree/deoalDOctree.h"

#include <dragengine/common/collection/decPointerList.h>

class deoalASpeaker;
class deoalAMicrophone;
class deoalAComponent;
class deoalASoundLevelMeter;
class deoalEnvProbe;
class deoalWorldOctreeVisitor;
class deoalDCollisionVolume;



/**
 * \brief OpenAL world octree.
 */
class deoalWorldOctree : public deoalDOctree{
private:
	decPointerList pComponents;
	decPointerList pSpeakers;
	decPointerList pMicrophones;
	decPointerList pEnvProbes;
	decPointerList pSoundLevelMeters;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create world octree. */
	deoalWorldOctree( const decDVector &center, const decDVector &halfSize );
	
	/** \brief Clean up world octree. */
	virtual ~deoalWorldOctree();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create octree for octant. */
	virtual deoalDOctree *CreateOctree( int octant ) const;
	
	/** \brief Clear content of node. */
	virtual void ClearNodeContent();
	
	/** \brief Clear all components from tree. */
	void ClearComponents();
	
	/** \brief Clear all speakers from tree. */
	void ClearSpeakers();
	
	/** \brief Clear all microphones from tree. */
	void ClearMicrophones();
	
	/** \brief Clear all environment probes from tree. */
	void ClearEnvProbes();
	
	/** \brief Clear all sound level meters from tree. */
	void ClearSoundLevelMeters();
	
	/** \brief Clear all. */
	void ClearAll();
	
	/** \brief Add component into octree. */
	void InsertComponentIntoTree( deoalAComponent *component, int maxDepth );
	
	/** \brief Add speaker into octree. */
	void InsertSpeakerIntoTree( deoalASpeaker *speaker, int maxDepth );
	
	/** \brief Add microphone into octree. */
	void InsertMicrophoneIntoTree( deoalAMicrophone *microphone, int maxDepth );
	
	/** \brief Add environment probe into octree. */
	void InsertEnvProbeIntoTree( deoalEnvProbe *envProbe, int maxDepth );
	
	/** \brief Add sound level meter into octree. */
	void InsertSoundLevelMeterIntoTree( deoalASoundLevelMeter *soundLevelMeter, int maxDepth );
	/*@}*/
	
	
	
	/** \name Components */
	/*@{*/
	/** \brief Number of components. */
	int GetComponentCount() const;
	
	/** \brief Component at index. */
	deoalAComponent *GetComponentAt( int index ) const;
	
	/** \brief Index of component or -1 if absent. */
	int IndexOfComponent( deoalAComponent *component ) const;
	
	/** \brief Add component. */
	void AddComponent( deoalAComponent *component );
	
	/** \brief Remove component. */
	void RemoveComponent( deoalAComponent *component );
	
	/** \brief Remove all components. */
	void RemoveAllComponents();
	/*@}*/
	
	
	
	/** \name Speakers */
	/*@{*/
	/** \brief Number of speakers. */
	int GetSpeakerCount() const;
	
	/** \brief Speaker at index. */
	deoalASpeaker *GetSpeakerAt( int index ) const;
	
	/** \brief Index of speaker or -1 if absent. */
	int IndexOfSpeaker( deoalASpeaker *speaker ) const;
	
	/** \brief Add speaker. */
	void AddSpeaker( deoalASpeaker *speaker );
	
	/** \brief Remove speaker. */
	void RemoveSpeaker( deoalASpeaker *speaker );
	
	/** \brief Remove all speakers. */
	void RemoveAllSpeakers();
	/*@}*/
	
	
	
	/** \name Microphones */
	/*@{*/
	/** \brief Number of microphones. */
	int GetMicrophoneCount() const;
	
	/** \brief Microphone at index. */
	deoalAMicrophone *GetMicrophoneAt( int index ) const;
	
	/** \brief Index of microphone or -1 if absent. */
	int IndexOfMicrophone( deoalAMicrophone *microphone ) const;
	
	/** \brief Add microphone. */
	void AddMicrophone( deoalAMicrophone *microphone );
	
	/** \brief Remove microphone. */
	void RemoveMicrophone( deoalAMicrophone *microphone );
	
	/** \brief Remove all microphones. */
	void RemoveAllMicrophones();
	/*@}*/
	
	
	
	/** \name Environment probes */
	/*@{*/
	/** \brief Number of environment probes. */
	int GetEnvProbeCount() const;
	
	/** \brief EnvProbe at index. */
	deoalEnvProbe *GetEnvProbeAt( int index ) const;
	
	/** \brief Index of environment probe or -1 if absent. */
	int IndexOfEnvProbe( deoalEnvProbe *microphone ) const;
	
	/** \brief Add environment probe. */
	void AddEnvProbe( deoalEnvProbe *microphone );
	
	/** \brief Remove environment probe. */
	void RemoveEnvProbe( deoalEnvProbe *microphone );
	
	/** \brief Remove all environment probes. */
	void RemoveAllEnvProbes();
	/*@}*/
	
	
	
	/** \name Sound level meters */
	/*@{*/
	/** \brief Number of sound level meters. */
	int GetSoundLevelMeterCount() const;
	
	/** \brief Sound level meter at index. */
	deoalASoundLevelMeter *GetSoundLevelMeterAt( int index ) const;
	
	/** \brief Index of sound level meter or -1 if absent. */
	int IndexOfSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter ) const;
	
	/** \brief Add sound level meter. */
	void AddSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	
	/** \brief Remove sound level meter. */
	void RemoveSoundLevelMeter( deoalASoundLevelMeter *soundLevelMeter );
	
	/** \brief Remove all sound level meters. */
	void RemoveAllSoundLevelMeters();
	/*@}*/
	
	
	
private:
	deoalWorldOctree *pGetNodeFor( const decDVector &center, const decDVector &halfSize, int maxDepth );
	deoalWorldOctree *pGetNodeFor( const decDVector &position, int maxDepth );
};

#endif

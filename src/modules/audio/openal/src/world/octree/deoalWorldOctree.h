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
	~deoalWorldOctree() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create octree for octant. */
	deoalDOctree *CreateOctree( int octant ) const override;
	
	/** \brief Clear content of node. */
	void ClearNodeContent() override;
	
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

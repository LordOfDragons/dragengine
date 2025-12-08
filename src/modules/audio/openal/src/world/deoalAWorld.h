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

#ifndef _DEOALAWORLD_H_
#define _DEOALAWORLD_H_

#include <dragengine/deObject.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/common/utils/decLayerMask.h>

class deoalAudioThread;
#include "../component/deoalAComponent.h"
#include "../microphone/deoalAMicrophone.h"
#include "../speaker/deoalASpeaker.h"
#include "../soundLevelMeter/deoalASoundLevelMeter.h"
class deoalWorldOctree;
class deoalWorldOctreeVisitor;


/**
 * Audio thread world.
 */
class deoalAWorld : public deObject{
private:
	deoalAudioThread &pAudioThread;
	
	deoalAComponent::Ref pRootComponent;
	deoalAComponent *pTailComponent;
	int pComponentCount;
	
	deoalASpeaker::Ref pRootSpeaker;
	deoalASpeaker *pTailSpeaker;
	int pSpeakerCount;
	
	deoalAMicrophone::Ref pRootMicrophone;
	deoalAMicrophone *pTailMicrophone;
	int pMicrophoneCount;
	
	deoalASoundLevelMeter::Ref pRootSoundLevelMeter;
	deoalASoundLevelMeter *pTailSoundLevelMeter;
	int pSoundLevelMeterCount;
	
	deoalWorldOctree *pOctree;
	decLayerMask pAllMicLayerMask;
	float pSpeakerGain;
	
	
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<deoalAWorld> Ref;


	/** \name Constructors and Destructors */
	/*@{*/
	/** Create world peer. */
	deoalAWorld(deoalAudioThread &audioThread, const decDVector &size);
	
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
	void VisitRegion(const decDVector &center, const decDVector &halfSize,
		deoalWorldOctreeVisitor &visitor);
	
	/** Layer mask containing all bits of all microphones. */
	inline const decLayerMask &GetAllMicLayerMask() const{ return pAllMicLayerMask; }
	
	/** Speaker gain. */
	inline float GetSpeakerGain() const{ return pSpeakerGain; }
	
	/** Set speaker gain. */
	void SetSpeakerGain(float gain);
	
	
	
	/** Invalidate environment probes. */
	void InvalidateEnvProbes(const decDVector &minExtend, const decDVector &maxExtend,
		const decLayerMask &layerMask);
	
	
	
	/** World is audible. */
	bool IsAudible() const;
	
	/**
	 * Enable or disable all speakers in the world.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetAllSpeakersEnabled(bool enabled);
	
	/**
	 * Set all microphone layer mask.
	 * \warning Called during synchronization time from main thread.
	 */
	void SetAllMicLayerMask(const decLayerMask &layerMask);
	
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
	inline const deoalAComponent::Ref &GetRootComponent() const{ return pRootComponent; }
	
	/** Add component. */
	void AddComponent(deoalAComponent *component);
	
	/** Remove component. */
	void RemoveComponent(deoalAComponent *component);
	
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
	inline const deoalASpeaker::Ref &GetRootSpeaker() const{ return pRootSpeaker; }
	
	/**
	 * Add speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void AddSpeaker(deoalASpeaker *speaker);
	
	/**
	 * Remove speaker.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveSpeaker(deoalASpeaker *speaker);
	
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
	inline const deoalAMicrophone::Ref &GetRootMicrophone() const{ return pRootMicrophone; }
	
	/** Add microphone. */
	void AddMicrophone(deoalAMicrophone *microphone);
	
	/** Remove microphone. */
	void RemoveMicrophone(deoalAMicrophone *microphone);
	
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
	inline const deoalASoundLevelMeter::Ref &GetRootSoundLevelMeter() const{ return pRootSoundLevelMeter; }
	
	/**
	 * Add sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void AddSoundLevelMeter(deoalASoundLevelMeter *soundLevelMeter);
	
	/**
	 * Remove sound level meter.
	 * \warning Called during synchronization time from main thread.
	 */
	void RemoveSoundLevelMeter(deoalASoundLevelMeter *soundLevelMeter);
	
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

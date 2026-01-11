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

#ifndef _DEOALSOUNDLEVELMETER_H_
#define _DEOALSOUNDLEVELMETER_H_

#include "deoalASoundLevelMeter.h"
#include "../speaker/deoalSpeakerList.h"

#include <dragengine/common/collection/decTSet.h>
#include <dragengine/common/collection/decTLinkedList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/systems/modules/audio/deBaseAudioSoundLevelMeter.h>

class deAudioOpenAL;
class deoalWorld;

class deSoundLevelMeter;



/**
 * \brief Sound level meter resource peer.
 */
class deoalSoundLevelMeter : public deBaseAudioSoundLevelMeter{
private:
	deAudioOpenAL &pOal;
	deSoundLevelMeter &pSoundLevelMeter;
	deoalASoundLevelMeter::Ref pASoundLevelMeter;
	
	deoalWorld *pParentWorld;
	
	bool pDirtyGeometry;
	bool pDirtyOctreeNode;
	bool pDirtyLayerMask;
	bool pDirtyEnabled;
	
	deSoundLevelMeter::cAudibleSpeaker *pSpeakers;
	int pSpeakerCount;
	int pSpeakerSize;
	
	decTObjectSet<deSpeaker> pOldSpeakers;
	
	decTLinkedList<deoalSoundLevelMeter>::Element pLLSyncWorld;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound level meter. */
	deoalSoundLevelMeter(deAudioOpenAL &oal, deSoundLevelMeter &soundLevelMeter);
	
	/** \brief Clean up sound level meter. */
	~deoalSoundLevelMeter() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deAudioOpenAL &GetOal() const{ return pOal; }
	
	/** \brief SoundLevelMeter. */
	inline deSoundLevelMeter &GetSoundLevelMeter() const{ return pSoundLevelMeter; }
	
	/** \brief Audio sound level meter. */
	inline const deoalASoundLevelMeter::Ref &GetASoundLevelMeter() const{ return pASoundLevelMeter; }
	
	
	
	/** \brief Parent world or NULL. */
	inline deoalWorld *GetParentWorld() const{ return pParentWorld; }
	
	/** \brief Set parent world or NULL. */
	void SetParentWorld(deoalWorld *world);
	
	
	
	/** \brief Number of speakers. */
	inline int GetSpeakerCount() const{ return pSpeakerCount; }
	
	/** \brief Speaker at index. */
	const deSoundLevelMeter::cAudibleSpeaker &GetSpeakerAt(int index) const;
	
	/** \brief Add speaker. */
	void AddSpeaker(deSpeaker *speaker, float volume);
	
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
	void TypeChanged() override;
	
	/** \brief Position changed. */
	void PositionChanged() override;
	
	/** \brief Orientation changed. */
	void OrientationChanged() override;
	
	/** \brief Cone angle changed. */
	void ConeAngleChanged() override;
	
	/** \brief Audible distance changed. */
	void AudibleDistanceChanged() override;
	
	/** \brief Layer mask changed. */
	void LayerMaskChanged() override;
	
	/** \brief Enabled changed. */
	void EnabledChanged() override;
	
	/** \brief Number of audible speakers. */
	int GetAudibleSpeakerCount() override;
	
	/** \brief Audible speaker at index. */
	const deSoundLevelMeter::cAudibleSpeaker &GetAudibleSpeakerAt(int index) override;
	/*@}*/
	
	
	
	/** \name Linking */
	/*@{*/
	/** \brief World syncing linked list. */
	inline decTLinkedList<deoalSoundLevelMeter>::Element &GetLLSyncWorld(){ return pLLSyncWorld; }
	inline const decTLinkedList<deoalSoundLevelMeter>::Element &GetLLSyncWorld() const{ return pLLSyncWorld; }
	/*@}*/
	
	
	
private:
	void pRequiresSync();
	void pUpdateAudibleSpeakers();
	int pIndexOfSpeaker(deSpeaker *speaker) const;
};

#endif

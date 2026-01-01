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

#ifndef _SYNESYNTHESIZER_H_
#define _SYNESYNTHESIZER_H_

#include "syneSynthesizerNotifier.h"
#include "controller/syneController.h"
#include "link/syneLink.h"
#include "source/syneSource.h"

#include <deigde/editableentity/igdeEditableEntity.h>


#include <dragengine/common/collection/decTOrderedSet.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerInstance.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deMicrophone.h>

class syneEffect;
class syneLoadSaveSystem;

class igdeEnvironment;



/**
 * \brief Synthesizer.
 */
class syneSynthesizer : public igdeEditableEntity{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<syneSynthesizer> Ref;
	
	
private:
	syneLoadSaveSystem &pLoadSaveSystem;
	
	deWorld::Ref pEngWorld;
	
	deSynthesizer::Ref pEngSynthesizer;
	deSynthesizerInstance::Ref pEngSynthesizerInstance;
	deSpeaker::Ref pEngSpeaker;
	deMicrophone::Ref pEngMicrophone;
	
	syneController::List pControllers;
	syneController::Ref pActiveController;
	
	syneLink::List pLinks;
	syneLink::Ref pActiveLink;
	
	syneSource::List pSources;
	syneSource::Ref pActiveSource;
	
	int pChannelCount;
	int pSampleRate;
	int pBytesPerSample;
	int pSampleCount;
	float pPlayTime;
	
	decTObjectOrderedSet<syneSynthesizerNotifier> pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer. */
	syneSynthesizer(igdeEnvironment *environment, syneLoadSaveSystem &lssys);
	
	/** \brief Clean up synthesizer. */
protected:
	virtual ~syneSynthesizer();
public:
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load save system. */
	inline syneLoadSaveSystem &GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	
	/** \brief Engine world. */
	inline const deWorld::Ref &GetEngineWorld() const{ return pEngWorld; }
	
	/** \brief Engine synthesizer. */
	inline const deSynthesizer::Ref &GetEngineSynthesizer() const{ return pEngSynthesizer; }
	
	/** \brief Engine synthesizer instance. */
	inline const deSynthesizerInstance::Ref &GetEngineSynthesizerInstance() const{ return pEngSynthesizerInstance; }
	
	
	
	/** \brief Number of channels. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Set number of channels. */
	void SetChannelCount(int channelCount);
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Set sample rate. */
	void SetSampleRate(int sampleRate);
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Set bytes per sample. */
	void SetBytesPerSample(int bytesPerSample);
	
	/** \brief Number of samples to create. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Set number of samples to create. */
	void SetSampleCount(int sampleCount);
	
	/** \brief Play time in seconds. */
	inline float GetPlayTime() const{ return pPlayTime; }
	
	
	
	/** \brief Synthesizer is looping. */
	bool GetLooping() const;
	
	/** \brief Set if synthesizer is looping. */
	void SetLooping(bool looping);
	
	/** \brief Synthesizer is stopped. */
	bool GetStopped() const;
	
	/** \brief Synthesizer is paused. */
	bool GetPaused() const;
	
	/** \brief Synthesizer is playing. */
	bool GetPlaying() const;
	
	/** \brief Start playing synthesizer. */
	void Play();
	
	/** \brief Pause playing synthesizer. */
	void Pause();
	
	/** \brief Stop playing synthesizer. */
	void Stop();
	
	/** \brief Playback speed. */
	float GetSpeed() const;
	
	/** \brief Set playback speed. */
	void SetSpeed(float speed);
	
	/** \brief Volume. */
	float GetVolume() const;
	
	/** \brief Set volume. */
	void SetVolume(float volume);
	
	
	
	/** \brief Dispose of all resources. */
	void Dispose();
	
	/** \brief Update. */
	void Update(float elapsed);
	
	/** \brief Reset synthesizer. */
	void Reset();
	
	/** \brief Activate microphone. */
	void ActivateMicrophone();
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Controllers. */
	inline const syneController::List &GetControllers() const{ return pControllers; }
	
	/** \brief Add controller. */
	void AddController(syneController *controller);
	
	/** \brief Insert controller. */
	void InsertControllerAt(syneController *controller, int index);
	
	/** \brief Move controllerposition. */
	void MoveControllerTo(syneController *controller, int index);
	
	/** \brief Remove controller. */
	void RemoveController(syneController *controller);
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Active controller or \em nullptr if not set. */
	inline const syneController::Ref &GetActiveController() const{ return pActiveController; }
	
	/** \brief Set active controller or \em nullptr if not set. */
	void SetActiveController(syneController *controller);
	/*@}*/
	
	
	
	/** \name Links */
	/*@{*/
	/** \brief Links. */
	inline const syneLink::List &GetLinks() const{ return pLinks; }
	
	/** \brief Add link. */
	void AddLink(syneLink *link);
	
	/** \brief Insert link. */
	void InsertLinkAt(syneLink *link, int index);
	
	/** \brief Move link. */
	void MoveLinkTo(syneLink *link, int index);
	
	/** \brief Remove link. */
	void RemoveLink(syneLink *link);
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Active link or \em nullptr if not set. */
	inline const syneLink::Ref &GetActiveLink() const{ return pActiveLink; }
	
	/** \brief Set active link or \em nullptr if not set. */
	void SetActiveLink(syneLink *link);
	
	/** \brief Number of targets using link. */
	int CountLinkUsage(syneLink *link) const;
	/*@}*/
	
	
	
	/** \name Sources */
	/*@{*/
	/** \brief Sources. */
	inline const syneSource::List &GetSources() const{ return pSources; }
	
	/** \brief Add source. */
	void AddSource(syneSource *source);
	
	/** \brief Insert source. */
	void InsertSourceAt(syneSource *source, int index);
	
	/** \brief Move source. */
	void MoveSourceTo(syneSource *source, int index);
	
	/** \brief Remove source. */
	void RemoveSource(syneSource *source);
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	/** \brief Active source or \em nullptr if not set. */
	inline const syneSource::Ref &GetActiveSource() const{ return pActiveSource; }
	
	/** \brief Set active source or \em nullptr if not set. */
	void SetActiveSource(syneSource *source);
	
	/** \brief Rebuild sources. */
	void RebuildSources();
	
	/** \brief Directory changed. */
	void DirectoryChanged();
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add notifier. */
	void AddNotifier(syneSynthesizerNotifier *notifier);
	
	/** \brief Remove notifier. */
	void RemoveNotifier(syneSynthesizerNotifier *notifier);
	
	
	
	/** \brief Notify listeners the changed or saved state changed. */
	virtual void NotifyStateChanged();
	
	/** \brief Notify listeners undo system changed. */
	virtual void NotifyUndoChanged();
	
	/** \brief Notify listeners synthesizer changed. */
	void NotifySynthesizerChanged();
	
	/** \brief Notify listeners playback parameters changed. */
	void NotifyPlaybackChanged();
	
	
	
	/** \brief Notify listeners active controller changed. */
	void NotifyActiveControllerChanged();
	
	/** \brief Notify listeners controller changed. */
	void NotifyControllerChanged(syneController *controller);
	
	/** \brief Notify listeners controller name changed. */
	void NotifyControllerNameChanged(syneController *controller);
	
	/** \brief Notify listeners controller range changed. */
	void NotifyControllerRangeChanged(syneController *controller);
	
	/** \brief Notify listeners controller curve changed. */
	void NotifyControllerCurveChanged(syneController *controller);
	
	/** \brief Notify listeners order or count of controllers changed. */
	void NotifyControllerStructureChanged();
	
	
	
	/** \brief Notify listeners active link changed. */
	void NotifyActiveLinkChanged();
	
	/** \brief Notify listeners link changed. */
	void NotifyLinkChanged(syneLink *link);
	
	/** \brief Notify listeners link name changed. */
	void NotifyLinkNameChanged(syneLink *link);
	
	/** \brief Notify listeners order or count of links changed. */
	void NotifyLinkStructureChanged();
	
	
	
	/** \brief Notify listeners active source changed. */
	void NotifyActiveSourceChanged();
	
	/** \brief Notify listeners source changed. */
	void NotifySourceChanged(syneSource *source);
	
	/** \brief Notify listeners source name changed. */
	void NotifySourceNameChanged(syneSource *source);
	
	/** \brief Notify listeners order or count of sources changed. */
	void NotifySourceStructureChanged();
	
	/** \brief Notify listeners active effect changed. */
	void NotifyActiveEffectChanged(syneSource *source);
	
	/** \brief Notify listeners effect changed. */
	void NotifyEffectChanged(syneSource *source, syneEffect *effect);
	
	/** \brief Notify listeners order or count of effects changed. */
	void NotifyEffectStructureChanged(syneSource *source);
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateWorld();
	void pUpdateLinks();
	void pUpdateEngineControllers();
	void pUpdatePlayTime();
};

#endif

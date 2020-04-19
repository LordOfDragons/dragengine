/* 
 * Drag[en]gine IGDE Synthesizer Editor
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

#ifndef _SESYNTHESIZER_H_
#define _SESYNTHESIZER_H_

#include "controller/seControllerList.h"
#include "link/seLinkList.h"
#include "source/seSourceList.h"

#include <deigde/editableentity/igdeEditableEntity.h>

#include <dragengine/common/collection/decObjectSet.h>

class seEffect;
class seLoadSaveSystem;

class seSynthesizerNotifier;

class igdeEnvironment;

class deWorld;
class deSynthesizer;
class deSynthesizerInstance;
class deSpeaker;
class deMicrophone;



/**
 * \brief Synthesizer.
 */
class seSynthesizer : public igdeEditableEntity{
private:
	seLoadSaveSystem &pLoadSaveSystem;
	
	deWorld *pEngWorld;
	
	deSynthesizer *pEngSynthesizer;
	deSynthesizerInstance *pEngSynthesizerInstance;
	deSpeaker *pEngSpeaker;
	deMicrophone *pEngMicrophone;
	
	seControllerList pControllers;
	seController *pActiveController;
	
	seLinkList pLinks;
	seLink *pActiveLink;
	
	seSourceList pSources;
	seSource *pActiveSource;
	
	int pChannelCount;
	int pSampleRate;
	int pBytesPerSample;
	int pSampleCount;
	float pPlayTime;
	
	decObjectSet pListeners;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create synthesizer. */
	seSynthesizer( igdeEnvironment *environment, seLoadSaveSystem &lssys );
	
	/** \brief Clean up synthesizer. */
	virtual ~seSynthesizer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Load save system. */
	inline seLoadSaveSystem &GetLoadSaveSystem() const{ return pLoadSaveSystem; }
	
	/** \brief Engine world. */
	inline deWorld *GetEngineWorld() const{ return pEngWorld; }
	
	/** \brief Engine synthesizer. */
	inline deSynthesizer *GetEngineSynthesizer() const{ return pEngSynthesizer; }
	
	/** \brief Engine synthesizer instance. */
	inline deSynthesizerInstance *GetEngineSynthesizerInstance() const{ return pEngSynthesizerInstance; }
	
	
	
	/** \brief Number of channels. */
	inline int GetChannelCount() const{ return pChannelCount; }
	
	/** \brief Set number of channels. */
	void SetChannelCount( int channelCount );
	
	/** \brief Sample rate. */
	inline int GetSampleRate() const{ return pSampleRate; }
	
	/** \brief Set sample rate. */
	void SetSampleRate( int sampleRate );
	
	/** \brief Bytes per sample. */
	inline int GetBytesPerSample() const{ return pBytesPerSample; }
	
	/** \brief Set bytes per sample. */
	void SetBytesPerSample( int bytesPerSample );
	
	/** \brief Number of samples to create. */
	inline int GetSampleCount() const{ return pSampleCount; }
	
	/** \brief Set number of samples to create. */
	void SetSampleCount( int sampleCount );
	
	/** \brief Play time in seconds. */
	inline float GetPlayTime() const{ return pPlayTime; }
	
	
	
	/** \brief Synthesizer is looping. */
	bool GetLooping() const;
	
	/** \brief Set if synthesizer is looping. */
	void SetLooping( bool looping );
	
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
	void SetSpeed( float speed );
	
	/** \brief Volume. */
	float GetVolume() const;
	
	/** \brief Set volume. */
	void SetVolume( float volume );
	
	
	
	/** \brief Dispose of all resources. */
	void Dispose();
	
	/** \brief Update. */
	void Update( float elapsed );
	
	/** \brief Reset synthesizer. */
	void Reset();
	
	/** \brief Activate microphone. */
	void ActivateMicrophone();
	/*@}*/
	
	
	
	/** \name Controllers */
	/*@{*/
	/** \brief Controllers. */
	inline const seControllerList &GetControllers() const{ return pControllers; }
	
	/** \brief Add controller. */
	void AddController( seController *controller );
	
	/** \brief Insert controller. */
	void InsertControllerAt( seController *controller, int index );
	
	/** \brief Move controllerposition. */
	void MoveControllerTo( seController *controller, int index );
	
	/** \brief Remove controller. */
	void RemoveController( seController *controller );
	
	/** \brief Remove all controllers. */
	void RemoveAllControllers();
	
	/** \brief Active controller or \em NULL if not set. */
	inline seController *GetActiveController() const{ return pActiveController; }
	
	/** \brief Set active controller or \em NULL if not set. */
	void SetActiveController( seController *controller );
	/*@}*/
	
	
	
	/** \name Links */
	/*@{*/
	/** \brief Links. */
	inline const seLinkList &GetLinks() const{ return pLinks; }
	
	/** \brief Add link. */
	void AddLink( seLink *link );
	
	/** \brief Insert link. */
	void InsertLinkAt( seLink *link, int index );
	
	/** \brief Move link. */
	void MoveLinkTo( seLink *link, int index );
	
	/** \brief Remove link. */
	void RemoveLink( seLink *link );
	
	/** \brief Remove all links. */
	void RemoveAllLinks();
	
	/** \brief Active link or \em NULL if not set. */
	inline seLink *GetActiveLink() const{ return pActiveLink; }
	
	/** \brief Set active link or \em NULL if not set. */
	void SetActiveLink( seLink *link );
	
	/** \brief Number of targets using link. */
	int CountLinkUsage( seLink *link ) const;
	/*@}*/
	
	
	
	/** \name Sources */
	/*@{*/
	/** \brief Sources. */
	inline const seSourceList &GetSources() const{ return pSources; }
	
	/** \brief Add source. */
	void AddSource( seSource *source );
	
	/** \brief Insert source. */
	void InsertSourceAt( seSource *source, int index );
	
	/** \brief Move source. */
	void MoveSourceTo( seSource *source, int index );
	
	/** \brief Remove source. */
	void RemoveSource( seSource *source );
	
	/** \brief Remove all sources. */
	void RemoveAllSources();
	
	/** \brief Active source or \em NULL if not set. */
	inline seSource *GetActiveSource() const{ return pActiveSource; }
	
	/** \brief Set active source or \em NULL if not set. */
	void SetActiveSource( seSource *source );
	
	/** \brief Rebuild sources. */
	void RebuildSources();
	
	/** \brief Directory changed. */
	void DirectoryChanged();
	/*@}*/
	
	
	
	/** \name Notifiers */
	/*@{*/
	/** \brief Add notifier. */
	void AddNotifier( seSynthesizerNotifier *notifier );
	
	/** \brief Remove notifier. */
	void RemoveNotifier( seSynthesizerNotifier *notifier );
	
	
	
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
	void NotifyControllerChanged( seController *controller );
	
	/** \brief Notify listeners controller name changed. */
	void NotifyControllerNameChanged( seController *controller );
	
	/** \brief Notify listeners controller range changed. */
	void NotifyControllerRangeChanged( seController *controller );
	
	/** \brief Notify listeners controller curve changed. */
	void NotifyControllerCurveChanged( seController *controller );
	
	/** \brief Notify listeners order or count of controllers changed. */
	void NotifyControllerStructureChanged();
	
	
	
	/** \brief Notify listeners active link changed. */
	void NotifyActiveLinkChanged();
	
	/** \brief Notify listeners link changed. */
	void NotifyLinkChanged( seLink *link );
	
	/** \brief Notify listeners link name changed. */
	void NotifyLinkNameChanged( seLink *link );
	
	/** \brief Notify listeners order or count of links changed. */
	void NotifyLinkStructureChanged();
	
	
	
	/** \brief Notify listeners active source changed. */
	void NotifyActiveSourceChanged();
	
	/** \brief Notify listeners source changed. */
	void NotifySourceChanged( seSource *source );
	
	/** \brief Notify listeners source name changed. */
	void NotifySourceNameChanged( seSource *source );
	
	/** \brief Notify listeners order or count of sources changed. */
	void NotifySourceStructureChanged();
	
	/** \brief Notify listeners active effect changed. */
	void NotifyActiveEffectChanged( seSource *source );
	
	/** \brief Notify listeners effect changed. */
	void NotifyEffectChanged( seSource *source, seEffect *effect );
	
	/** \brief Notify listeners order or count of effects changed. */
	void NotifyEffectStructureChanged( seSource *source );
	/*@}*/
	
	
	
private:
	void pCleanUp();
	
	void pCreateWorld();
	void pUpdateLinks();
	void pUpdateEngineControllers();
	void pUpdatePlayTime();
};

#endif

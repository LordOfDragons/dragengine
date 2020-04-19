/* 
 * Drag[en]gine Null Audio Module
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

// include only once
#ifndef _DEAUDIONULL_H_
#define _DEAUDIONULL_H_

// includes
#include "dragengine/systems/modules/audio/deBaseAudioModule.h"



/**
 * @brief Null Audio Module.
 * Null audio output no audio.
 */
class deAudioNull : public deBaseAudioModule{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new null audio module. */
	deAudioNull( deLoadableModule &loadableModule );
	/** Cleans up the null audio module. */
	virtual ~deAudioNull();
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Called to init the module and prepare it for audio. Returns true on success.
	 * To access the os of the engine use the GetOS function. The active microphone
	 * is passed as parameter and can be NULL if no microphone is active yet.
	 */
	virtual bool Init( deMicrophone *activeMic );
	/**
	 * Called to cleanup the module. All resources have to be freed and running threads
	 * stopped or killed if needed.
	 */
	virtual void CleanUp();
	/** Process audio. */
	virtual void ProcessAudio();
	/**
	 * Sets the active microphone. Can be NULL to unset the active microphone.
	 */
	virtual void SetActiveMicrophone( deMicrophone *microphone );
	/*@}*/
	
	/** @name Audio Management */
	/*@{*/
	virtual deBaseAudioWorld *CreateWorld( deWorld *world );
	virtual deBaseAudioSound *CreateSound( deSound *sound );
	virtual deBaseAudioSpeaker *CreateSpeaker( deSpeaker *speaker );
	virtual deBaseAudioMicrophone *CreateMicrophone( deMicrophone *microphone );
	virtual deBaseAudioComponent *CreateComponent( deComponent *component );
	virtual deBaseAudioModel *CreateModel( deModel *model );
	virtual deBaseAudioSkin *CreateSkin( deSkin *skin );
	virtual deBaseAudioDecal *CreateDecal( deDecal *decal );
	virtual deBaseAudioSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter );
	virtual deBaseAudioVideoPlayer *CreateVideoPlayer( deVideoPlayer *videoPlayer );
	virtual deBaseAudioSynthesizerInstance *CreateSynthesizerInstance( deSynthesizerInstance *instance );
	virtual deBaseAudioHeightTerrain *CreateHeightTerrain( deHeightTerrain &heightTerrain );
	/*@}*/
};

// end of include only once
#endif

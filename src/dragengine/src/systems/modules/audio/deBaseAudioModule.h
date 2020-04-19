/* 
 * Drag[en]gine Game Engine
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

#ifndef _DEBASEAUDIOMODULE_H_
#define _DEBASEAUDIOMODULE_H_

#include "../deBaseModule.h"

class deBaseAudioComponent;
class deBaseAudioDecal;
class deBaseAudioHeightTerrain;
class deBaseAudioMicrophone;
class deBaseAudioModel;
class deBaseAudioSkin;
class deBaseAudioSound;
class deBaseAudioSoundLevelMeter;
class deBaseAudioSpeaker;
class deBaseAudioSynthesizerInstance;
class deBaseAudioVideoPlayer;
class deBaseAudioWorld;

class deComponent;
class deDecal;
class deHeightTerrain;
class deMicrophone;
class deModel;
class deSkin;
class deSound;
class deSoundLevelMeter;
class deSpeaker;
class deSynthesizerInstance;
class deVideoPlayer;
class deWorld;


/**
 * \brief Base audio module.
 */
class deBaseAudioModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseAudioModule( deLoadableModule &loadableModule );
	
	/** \brief Clean up module. */
	virtual ~deBaseAudioModule();
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Init module and prepare for audio processing.
	 * \param[in] activeMic Initial active microphone or NULL.
	 * \note To access the os of the engine use the GetOS function.
	 * \returns true on success or false on failure.
	 */
	virtual bool Init( deMicrophone *activeMic ) = 0;
	
	/**
	 * \brief Clean up module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	
	/** \brief Process audio. */
	virtual void ProcessAudio() = 0;
	
	/** \brief Set active microphone or NULL. */
	virtual void SetActiveMicrophone( deMicrophone *microphone ) = 0;
	/*@}*/
	
	
	
	/** \name Audio Management */
	/*@{*/
	/** \brief Creat peer for world. */
	virtual deBaseAudioWorld *CreateWorld( deWorld *world ) = 0;
	
	/** \brief Create peer for sound. */
	virtual deBaseAudioSound *CreateSound( deSound *sound ) = 0;
	
	/** \brief Create peer for speaker. */
	virtual deBaseAudioSpeaker *CreateSpeaker( deSpeaker *speaker ) = 0;
	
	/** \brief Create peer for microphone. */
	virtual deBaseAudioMicrophone *CreateMicrophone( deMicrophone *microphone ) = 0;
	
	/** \brief Create peer for component. */
	virtual deBaseAudioComponent *CreateComponent( deComponent *component ) = 0;
	
	/** \brief Create peer for skin. */
	virtual deBaseAudioSkin *CreateSkin( deSkin *skin ) = 0;
	
	/** \brief Create peer for model. */
	virtual deBaseAudioModel *CreateModel( deModel *model ) = 0;
	
	/** \brief Create peer for decal. */
	virtual deBaseAudioDecal *CreateDecal( deDecal *decal ) = 0;
	
	/** \brief Create deSoundLevelMeter peer. */
	virtual deBaseAudioSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter ) = 0;
	
	/** \brief Create deVideoPlayer peer. */
	virtual deBaseAudioVideoPlayer *CreateVideoPlayer( deVideoPlayer *videoPlayer ) = 0;
	
	/** \brief Create deSynthesizerInstance peer. */
	virtual deBaseAudioSynthesizerInstance *CreateSynthesizerInstance( deSynthesizerInstance *instance ) = 0;
	
	/** \brief Create deHeightTerrain peer. */
	virtual deBaseAudioHeightTerrain *CreateHeightTerrain( deHeightTerrain &heightTerrain ) = 0;
	/*@}*/
};

#endif

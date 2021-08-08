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

#ifndef _DEAUDIOSYSTEM_H_
#define _DEAUDIOSYSTEM_H_

#include "deBaseSystem.h"
#include "../common/math/decMath.h"

class deBaseAudioModule;
class deComponent;
class deDecal;
class deMicrophone;
class deModel;
class deSkin;
class deSound;
class deSpeaker;
class deWorld;
class deSoundLevelMeter;
class deSynthesizerInstance;
class deVideoPlayer;
class deHeightTerrain;


/**
 * \brief Audio System.
 *
 * The Audio System is a single type system providing the output functions
 * of the engine in the audio direction.
 */
class deAudioSystem : public deBaseSystem{
private:
	deBaseAudioModule *pActiveModule;
	deMicrophone *pActiveMic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new audio system  linked to the given engine. */
	deAudioSystem( deEngine *engine );
	
	/** \brief Clean up audio system. */
	virtual ~deAudioSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activated audio module. */
	inline deBaseAudioModule *GetActiveModule() const{ return pActiveModule; }
	
	/** \brief Active microphone. */
	inline deMicrophone *GetActiveMicrophone() const{ return pActiveMic; }
	
	/** \brief Set active microphone or NULL to unset it. */
	void SetActiveMicrophone( deMicrophone *microphone );
	/*@}*/
	
	
	
	/**
	 * \name Runtime Functions
	 * Run-Time functions can only be called if the audio system
	 * is running. Otherwise those functions cause an exception.
	 */
	/*@{*/
	/** \brief Process audio. */
	void ProcessAudio();
	
	/** \brief Create peer object for the given world using the active module and assigns it. */
	void LoadWorld( deWorld *world );
	
	/** \brief Create peer object for the given sound using the active module and assigns it. */
	void LoadSound( deSound *sound );
	
	/** \brief Create peer object for the given speaker using the active module and assigns it. */
	void LoadSpeaker( deSpeaker *speaker );
	
	/** \brief Create peer object for the given microphone using the active module and assigns it. */
	void LoadMicrophone( deMicrophone *microphone );
	
	/** \brief Create peer object for the given component using the active module and assigns it. */
	void LoadComponent( deComponent *component );
	
	/** \brief Create peer object for the given model using the active module and assigns it. */
	void LoadModel( deModel *model );
	
	/** \brief Create peer object for the given skin using the active module and assigns it. */
	void LoadSkin( deSkin *skin );
	
	/** \brief Create peer object for the given decal using the active module and assigns it. */
	void LoadDecal( deDecal *decal );
	
	/** \brief Create deSoundLevelMeter peer using active module and assigns it. */
	void LoadSoundLevelMeter( deSoundLevelMeter *meter );
	
	/** \brief Create peer for the video player using the active module and assigns it. */
	void LoadVideoPlayer( deVideoPlayer *videoPlayer );
	
	/** \brief Create peer for the synthesizer instance using the active module and assigns it. */
	void LoadSynthesizerInstance( deSynthesizerInstance *instance );
	
	/** \brief Create peer for the height terrain using the active module and assigns it. */
	void LoadHeightTerrain( deHeightTerrain &heightTerrain );
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule( deLoadableModule *module );
	
	/**
	 * \brief Clearcross references and links that could lead to memory leaks.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void ClearPermanents();
	
	/** \brief Carry out here actions right after the system started up. */
	virtual void PostStart();
	
	/** \brief Carry out here actions right before the system shuts down. */
	virtual void PreStop();
	/*@}*/
	
	
	
private:
	void pCleanUp();
};

#endif

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

#ifndef _DEAUDIOOPENAL_H_
#define _DEAUDIOOPENAL_H_

#include <dragengine/systems/modules/audio/deBaseAudioModule.h>
#include <dragengine/common/collection/decObjectSet.h>

class deoalAudioThread;
class deoalCommandExecuter;
class deoalConfiguration;
class deoalDevMode;
class deoalParameterList;
class deoalMicrophone;
class deoalWorld;



/**
 * \brief OpenAL Audio Module.
 */
class deAudioOpenAL : public deBaseAudioModule{
private:
	deoalAudioThread *pAudioThread;
	
	deoalConfiguration *pConfiguration;
	deoalCommandExecuter *pCommandExecuter;
	deoalParameterList *pParameters;
	
	deoalDevMode *pDevMode;
	
	deoalMicrophone *pActiveMicrophone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create openal audio module. */
	deAudioOpenAL( deLoadableModule &loadableModule );
	
	/** \brief Clean up openal audio module. */
	virtual ~deAudioOpenAL();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Has audio thread. */
	bool HasAudioThread() const;
	
	/** \brief Audio thread. */
	inline deoalAudioThread &GetAudioThread() const{ return *pAudioThread; }
	
	
	
	/** \brief Active microphone. */
	inline deoalMicrophone *GetActiveMicrophone() const{ return pActiveMicrophone; }
	
	/** \brief Active world. */
	deoalWorld *GetActiveWorld() const;
	
	
	
	/**
	 * \brief Called to init the module and prepare it for audio.
	 * 
	 * Returns true on success. To access the os of the engine use the GetOS function.
	 * The active microphone is passed as parameter and can be NULL if no microphone
	 * is active yet.
	 */
	virtual bool Init( deMicrophone *activeMic );
	
	/**
	 * \brief Called to cleanup the module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp();
	
	/** \brief Process audio. */
	virtual void ProcessAudio();
	
	/**
	 * \brief Set active microphone.
	 * 
	 * Can be NULL to unset the active microphone.
	 */
	virtual void SetActiveMicrophone( deMicrophone *microphone );
	
	
	
	/** \brief Configuration. */
	inline deoalConfiguration &GetConfiguration() const{ return *pConfiguration; }
	/*@}*/
	
	
	
	/** \name Audio Management */
	/*@{*/
	/** \brief Create world peer. */
	virtual deBaseAudioWorld *CreateWorld( deWorld *world );
	
	/** \brief Create sound peer. */
	virtual deBaseAudioSound *CreateSound( deSound *sound );
	
	/** \brief Create speaker peer. */
	virtual deBaseAudioSpeaker *CreateSpeaker( deSpeaker *speaker );
	
	/** \brief Create microphone peer. */
	virtual deBaseAudioMicrophone *CreateMicrophone( deMicrophone *microphone );
	
	/** \brief Create component peer. */
	virtual deBaseAudioComponent *CreateComponent( deComponent *component );
	
	/** \brief Create model peer. */
	virtual deBaseAudioModel *CreateModel( deModel *model );
	
	/** \brief Create skin peer. */
	virtual deBaseAudioSkin *CreateSkin( deSkin *skin );
	
	/** \brief Create decal peer. */
	virtual deBaseAudioDecal *CreateDecal( deDecal *decal );
	
	/** \brief Create deSoundLevelMeter eer. */
	virtual deBaseAudioSoundLevelMeter *CreateSoundLevelMeter( deSoundLevelMeter *meter );
	
	/** \brief Create deVideoPlayer peer. */
	virtual deBaseAudioVideoPlayer *CreateVideoPlayer( deVideoPlayer *videoPlayer );
	
	/** \brief Create deSynthesizerInstance peer. */
	virtual deBaseAudioSynthesizerInstance *CreateSynthesizerInstance( deSynthesizerInstance *instance );
	
	/** \brief Create deHeightTerrain peer. */
	virtual deBaseAudioHeightTerrain *CreateHeightTerrain( deHeightTerrain &heightTerrain );
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Texture property map changed. */
	virtual void TexturePropertyMapChanged();
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Number of parameters. */
	virtual int GetParameterCount() const;
	
	/**
	 * \brief Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with informations about the parameter
	 */
	virtual void GetParameterInfo( int index, deModuleParameter &parameter ) const;
	
	/** \brief Index of named parameter or -1 if not found. */
	virtual int IndexOfParameterNamed( const char *name ) const;
	
	/** \brief Value of named parameter. */
	virtual decString GetParameterValue( const char *name ) const;
	
	/** \brief Set value of named parameter. */
	virtual void SetParameterValue( const char *name, const char *value );
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Send command to module. */
	virtual void SendCommand( const decUnicodeArgumentList &command, decUnicodeString &answer );
	
	/** \brief Developer mode or \em NULL. */
	inline deoalDevMode *GetDevMode() const{ return pDevMode; }
	/*@}*/
};

#endif

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
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	
	
	
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
	 * \param[in] parameter Object to fill with information about the parameter
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

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

#include "parameters/deoalParameter.h"

#include <dragengine/systems/modules/audio/deBaseAudioModule.h>

class deoalAudioThread;
class deoalCommandExecuter;
class deoalConfiguration;
class deoalDevMode;
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
	deoalParameter::List pParameters;
	
	deoalDevMode *pDevMode;
	
	deoalMicrophone *pActiveMicrophone;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create openal audio module. */
	deAudioOpenAL(deLoadableModule &loadableModule);
	
	/** \brief Clean up openal audio module. */
	~deAudioOpenAL() override;
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
	bool Init(deMicrophone *activeMic) override;
	
	/**
	 * \brief Called to cleanup the module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	void CleanUp() override;
	
	/** \brief Process audio. */
	void ProcessAudio() override;
	
	/**
	 * \brief Set active microphone.
	 * 
	 * Can be NULL to unset the active microphone.
	 */
	void SetActiveMicrophone(deMicrophone *microphone) override;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	int GetFPSRate() override;
	
	
	
	/** \brief Configuration. */
	inline deoalConfiguration &GetConfiguration() const{ return *pConfiguration; }
	/*@}*/
	
	
	
	/** \name Audio Management */
	/*@{*/
	/** \brief Create world peer. */
	deBaseAudioWorld *CreateWorld(deWorld *world) override;
	
	/** \brief Create sound peer. */
	deBaseAudioSound *CreateSound(deSound *sound) override;
	
	/** \brief Create speaker peer. */
	deBaseAudioSpeaker *CreateSpeaker(deSpeaker *speaker) override;
	
	/** \brief Create microphone peer. */
	deBaseAudioMicrophone *CreateMicrophone(deMicrophone *microphone) override;
	
	/** \brief Create component peer. */
	deBaseAudioComponent *CreateComponent(deComponent *component) override;
	
	/** \brief Create model peer. */
	deBaseAudioModel *CreateModel(deModel *model) override;
	
	/** \brief Create skin peer. */
	deBaseAudioSkin *CreateSkin(deSkin *skin) override;
	
	/** \brief Create decal peer. */
	deBaseAudioDecal *CreateDecal(deDecal *decal) override;
	
	/** \brief Create deSoundLevelMeter eer. */
	deBaseAudioSoundLevelMeter *CreateSoundLevelMeter(deSoundLevelMeter *meter) override;
	
	/** \brief Create deVideoPlayer peer. */
	deBaseAudioVideoPlayer *CreateVideoPlayer(deVideoPlayer *videoPlayer) override;
	
	/** \brief Create deSynthesizerInstance peer. */
	deBaseAudioSynthesizerInstance *CreateSynthesizerInstance(deSynthesizerInstance *instance) override;
	
	/** \brief Create deHeightTerrain peer. */
	deBaseAudioHeightTerrain *CreateHeightTerrain(deHeightTerrain &heightTerrain) override;
	/*@}*/
	
	
	
	/** \name Notifications */
	/*@{*/
	/** \brief Texture property map changed. */
	virtual void TexturePropertyMapChanged();
	/*@}*/
	
	
	
	/** \name Parameters */
	/*@{*/
	/** \brief Number of parameters. */
	int GetParameterCount() const override;
	
	/**
	 * \brief Get information about parameter.
	 * \param[in] index Index of the parameter
	 * \param[in] parameter Object to fill with information about the parameter
	 */
	void GetParameterInfo(int index, deModuleParameter &parameter) const override;
	
	/** \brief Index of named parameter or -1 if not found. */
	int IndexOfParameterNamed(const char *name) const override;
	
	/** \brief Value of named parameter. */
	decString GetParameterValue(const char *name) const override;
	
	/** \brief Set value of named parameter. */
	void SetParameterValue(const char *name, const char *value) override;
	/*@}*/
	
	
	
	/** \name Debugging */
	/*@{*/
	/** \brief Send command to module. */
	void SendCommand(const decUnicodeArgumentList &command, decUnicodeString &answer) override;
	
	/** \brief Developer mode or \em NULL. */
	inline deoalDevMode *GetDevMode() const{ return pDevMode; }
	/*@}*/
};

#endif

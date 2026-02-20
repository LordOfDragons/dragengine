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
class DE_DLL_EXPORT deBaseAudioModule : public deBaseModule{
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create module. */
	deBaseAudioModule(deLoadableModule &loadableModule);
	
	/** \brief Clean up module. */
	~deBaseAudioModule() override;
	/*@}*/
	
	
	
	/** \name Module Management */
	/*@{*/
	/**
	 * \brief Init module and prepare for audio processing.
	 * \param[in] activeMic Initial active microphone or NULL.
	 * \note To access the os of the engine use the GetOS function.
	 * \returns true on success or false on failure.
	 */
	virtual bool Init(deMicrophone *activeMic) = 0;
	
	/**
	 * \brief Clean up module.
	 * 
	 * All resources have to be freed and running threads stopped or killed if needed.
	 */
	virtual void CleanUp() = 0;
	
	/** \brief Process audio. */
	virtual void ProcessAudio() = 0;
	
	/** \brief Set active microphone or NULL. */
	virtual void SetActiveMicrophone(deMicrophone *microphone) = 0;
	
	/**
	 * \brief Frame-per-second rate averaged over the last couple of frames.
	 * \version 1.6
	 * 
	 * Returns 0 if module is not using a separate thread.
	 */
	virtual int GetFPSRate();
	/*@}*/
	
	
	
	/** \name Audio Management */
	/*@{*/
	/** \brief Creat peer for world. */
	virtual deBaseAudioWorld *CreateWorld(deWorld *world) = 0;
	
	/** \brief Create peer for sound. */
	virtual deBaseAudioSound *CreateSound(deSound *sound) = 0;
	
	/** \brief Create peer for speaker. */
	virtual deBaseAudioSpeaker *CreateSpeaker(deSpeaker *speaker) = 0;
	
	/** \brief Create peer for microphone. */
	virtual deBaseAudioMicrophone *CreateMicrophone(deMicrophone *microphone) = 0;
	
	/** \brief Create peer for component. */
	virtual deBaseAudioComponent *CreateComponent(deComponent *component) = 0;
	
	/** \brief Create peer for skin. */
	virtual deBaseAudioSkin *CreateSkin(deSkin *skin) = 0;
	
	/** \brief Create peer for model. */
	virtual deBaseAudioModel *CreateModel(deModel *model) = 0;
	
	/** \brief Create peer for decal. */
	virtual deBaseAudioDecal *CreateDecal(deDecal *decal) = 0;
	
	/** \brief Create deSoundLevelMeter peer. */
	virtual deBaseAudioSoundLevelMeter *CreateSoundLevelMeter(deSoundLevelMeter *meter) = 0;
	
	/** \brief Create deVideoPlayer peer. */
	virtual deBaseAudioVideoPlayer *CreateVideoPlayer(deVideoPlayer *videoPlayer) = 0;
	
	/** \brief Create deSynthesizerInstance peer. */
	virtual deBaseAudioSynthesizerInstance *CreateSynthesizerInstance(deSynthesizerInstance *instance) = 0;
	
	/** \brief Create deHeightTerrain peer. */
	virtual deBaseAudioHeightTerrain *CreateHeightTerrain(deHeightTerrain &heightTerrain) = 0;
	/*@}*/
};

#endif

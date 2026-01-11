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

// include only once
#ifndef _DEAUDIONULL_H_
#define _DEAUDIONULL_H_

// includes
#include <dragengine/systems/modules/audio/deBaseAudioModule.h>



/**
 * @brief Null Audio Module.
 * Null audio output no audio.
 */
class deAudioNull : public deBaseAudioModule{
public:
	/** @name Constructors and Destructors */
	/*@{*/
	/** Creates a new null audio module. */
	deAudioNull(deLoadableModule &loadableModule);
	/** Cleans up the null audio module. */
	~deAudioNull() override;
	/*@}*/
	
	/** @name Management */
	/*@{*/
	/**
	 * Called to init the module and prepare it for audio. Returns true on success.
	 * To access the os of the engine use the GetOS function. The active microphone
	 * is passed as parameter and can be NULL if no microphone is active yet.
	 */
	bool Init(deMicrophone *activeMic) override;
	/**
	 * Called to cleanup the module. All resources have to be freed and running threads
	 * stopped or killed if needed.
	 */
	void CleanUp() override;
	/** Process audio. */
	void ProcessAudio() override;
	/**
	 * Sets the active microphone. Can be NULL to unset the active microphone.
	 */
	void SetActiveMicrophone(deMicrophone *microphone) override;
	/*@}*/
	
	/** @name Audio Management */
	/*@{*/
	deBaseAudioWorld *CreateWorld(deWorld *world) override;
	deBaseAudioSound *CreateSound(deSound *sound) override;
	deBaseAudioSpeaker *CreateSpeaker(deSpeaker *speaker) override;
	deBaseAudioMicrophone *CreateMicrophone(deMicrophone *microphone) override;
	deBaseAudioComponent *CreateComponent(deComponent *component) override;
	deBaseAudioModel *CreateModel(deModel *model) override;
	deBaseAudioSkin *CreateSkin(deSkin *skin) override;
	deBaseAudioDecal *CreateDecal(deDecal *decal) override;
	deBaseAudioSoundLevelMeter *CreateSoundLevelMeter(deSoundLevelMeter *meter) override;
	deBaseAudioVideoPlayer *CreateVideoPlayer(deVideoPlayer *videoPlayer) override;
	deBaseAudioSynthesizerInstance *CreateSynthesizerInstance(deSynthesizerInstance *instance) override;
	deBaseAudioHeightTerrain *CreateHeightTerrain(deHeightTerrain &heightTerrain) override;
	/*@}*/
};

// end of include only once
#endif

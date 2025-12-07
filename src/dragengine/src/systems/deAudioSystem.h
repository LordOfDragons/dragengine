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
class DE_DLL_EXPORT deAudioSystem : public deBaseSystem{
private:
	deBaseAudioModule *pActiveModule;
	deMicrophone *pActiveMic;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new audio system  linked to the given engine. */
	deAudioSystem(deEngine *engine);
	
	/** \brief Clean up audio system. */
	virtual ~deAudioSystem();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Activated audio module. */
	inline deBaseAudioModule *GetActiveModule() const{return pActiveModule;}
	
	/** \brief Active microphone. */
	inline deMicrophone *GetActiveMicrophone() const{return pActiveMic;}
	
	/** \brief Set active microphone or NULL to unset it. */
	void SetActiveMicrophone(deMicrophone *microphone);
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
	void LoadWorld(deWorld *world);
	
	/** \brief Create peer object for the given sound using the active module and assigns it. */
	void LoadSound(deSound *sound);
	
	/** \brief Create peer object for the given speaker using the active module and assigns it. */
	void LoadSpeaker(deSpeaker *speaker);
	
	/** \brief Create peer object for the given microphone using the active module and assigns it. */
	void LoadMicrophone(deMicrophone *microphone);
	
	/** \brief Create peer object for the given component using the active module and assigns it. */
	void LoadComponent(deComponent *component);
	
	/** \brief Create peer object for the given model using the active module and assigns it. */
	void LoadModel(deModel *model);
	
	/** \brief Create peer object for the given skin using the active module and assigns it. */
	void LoadSkin(deSkin *skin);
	
	/** \brief Create peer object for the given decal using the active module and assigns it. */
	void LoadDecal(deDecal *decal);
	
	/** \brief Create deSoundLevelMeter peer using active module and assigns it. */
	void LoadSoundLevelMeter(deSoundLevelMeter *meter);
	
	/** \brief Create peer for the video player using the active module and assigns it. */
	void LoadVideoPlayer(deVideoPlayer *videoPlayer);
	
	/** \brief Create peer for the synthesizer instance using the active module and assigns it. */
	void LoadSynthesizerInstance(deSynthesizerInstance *instance);
	
	/** \brief Create peer for the height terrain using the active module and assigns it. */
	void LoadHeightTerrain(deHeightTerrain &heightTerrain);
	/*@}*/
	
	
	
	/** \name Overloadables */
	/*@{*/
	/**
	 * \brief Set active loadable module.
	 * 
	 * Do not forget to call the super function.
	 */
	virtual void SetActiveModule(deLoadableModule *module);
	
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

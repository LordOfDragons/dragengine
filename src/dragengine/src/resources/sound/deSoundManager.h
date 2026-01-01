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

#ifndef _DESOUNDMANAGER_H_
#define _DESOUNDMANAGER_H_

#include "deSound.h"
#include "deSoundDecoder.h"
#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"
#include "../../threading/deMutex.h"


/**
 * \brief Sound Resource Manager.
 */
class DE_DLL_EXPORT deSoundManager : public deFileResourceManager{
private:
	deFileResourceList pSounds;
	
	deSoundDecoder *pDecoderRoot;
	deSoundDecoder *pDecoderTail;
	int pDecoderCount;
	deMutex pMutexDecoder;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create sound resource manager. */
	deSoundManager(deEngine *engine);
	
	/** \brief Clean up sound resource manager and report leaks. */
	~deSoundManager() override;
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sounds. */
	int GetSoundCount() const;
	
	/** \brief Root sound resource for iteration purpose. */
	deSound *GetRootSound() const;
	
	/** \brief Sound with the given filename or NULL if not loaded yet. */
	deSound *GetSoundWith(const char *filename) const;
	
	/** \brief Sound with the given filename or NULL if not loaded yet. */
	deSound *GetSoundWith(deVirtualFileSystem *vfs, const char *filename) const;
	
	/** \brief Load sound from the given file relative to the given base path. */
	deSound::Ref LoadSound(const char *filename, const char *basePath, bool asynchron);
	
	/** \brief Load sound from the given file relative to the given base path. */
	deSound::Ref LoadSound(deVirtualFileSystem *vfs, const char *filename,
		const char *basePath, bool asynchron);
	
	/** \brief Save sound to the given file. */
	void SaveSound(deSound *sound, const char *filename);
	
	/** \brief Save sound to the given file. */
	void SaveSound(deVirtualFileSystem *vfs, deSound *sound, const char *filename);
	
	/**
	 * \brief Add loaded sound.
	 * \warning This method is to be used only by the resource loader!
	 */
	void AddLoadedSound(deSound *sound);
	
	
	
	/** \brief Create sound decoder. */
	deSoundDecoder::Ref CreateDecoder(deSound *sound);
	
	
	
	/** \brief Release leaking resources and report them. */
	void ReleaseLeakingResources() override;
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemAudioLoad() override;
	void SystemAudioUnload() override;
	
	/** \brief Synthesizer system peers of all stored resources have to be created. */
	void SystemSynthesizerLoad() override;
	
	/** \brief Synthesizer system peers of all stored resources have to be freed. */
	void SystemSynthesizerUnload() override;
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource(deResource *resource) override;
	void RemoveDecoder(deSoundDecoder *decoder);
	/*@}*/
};

#endif

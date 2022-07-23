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

#ifndef _DESOUNDMANAGER_H_
#define _DESOUNDMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"
#include "../../threading/deMutex.h"

class deSound;
class deSoundDecoder;


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
	deSoundManager( deEngine *engine );
	
	/** \brief Clean up sound resource manager and report leaks. */
	virtual ~deSoundManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of sounds. */
	int GetSoundCount() const;
	
	/** \brief Root sound resource for iteration purpose. */
	deSound *GetRootSound() const;
	
	/** \brief Sound with the given filename or NULL if not loaded yet. */
	deSound *GetSoundWith( const char *filename ) const;
	
	/** \brief Sound with the given filename or NULL if not loaded yet. */
	deSound *GetSoundWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Load sound from the given file relative to the given base path. */
	deSound *LoadSound( const char *filename, const char *basePath, bool asynchron );
	
	/** \brief Load sound from the given file relative to the given base path. */
	deSound *LoadSound( deVirtualFileSystem *vfs, const char *filename,
		const char *basePath, bool asynchron );
	
	/** \brief Save sound to the given file. */
	void SaveSound( deSound *sound, const char *filename );
	
	/** \brief Save sound to the given file. */
	void SaveSound( deVirtualFileSystem *vfs, deSound *sound, const char *filename );
	
	/**
	 * \brief Add loaded sound.
	 * \warning This method is to be used only by the resource loader!
	 */
	void AddLoadedSound( deSound *sound );
	
	
	
	/** \brief Create sound decoder. */
	deSoundDecoder *CreateDecoder( deSound *sound );
	
	
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	virtual void SystemAudioLoad();
	virtual void SystemAudioUnload();
	
	/** \brief Synthesizer system peers of all stored resources have to be created. */
	virtual void SystemSynthesizerLoad();
	
	/** \brief Synthesizer system peers of all stored resources have to be freed. */
	virtual void SystemSynthesizerUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	virtual void RemoveResource( deResource *resource );
	void RemoveDecoder( deSoundDecoder *decoder );
	/*@}*/
};

#endif

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

#ifndef _DEVIDEOMANAGER_H_
#define _DEVIDEOMANAGER_H_

#include "../deFileResourceManager.h"
#include "../deFileResourceList.h"
#include "../../threading/deMutex.h"

class deVideo;
class deVideoDecoder;
class deVideoAudioDecoder;


/**
 * \brief Video resource manager.
 */
class deVideoManager : public deFileResourceManager{
private:
	deFileResourceList pVideos;
	
	deVideoDecoder *pDecoderRoot;
	deVideoDecoder *pDecoderTail;
	int pDecoderCount;
	
	deVideoAudioDecoder *pAudioDecoderRoot;
	deVideoAudioDecoder *pAudioDecoderTail;
	int pAudioDecoderCount;
	
	deMutex pMutexDecoder;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create video resource manager. */
	deVideoManager( deEngine *engine );
	
	/** \brief Clean up video resource manager and reports leaking resources. */
	virtual ~deVideoManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of videos. */
	int GetVideoCount() const;
	
	/** \brief Root video resource for iteration purpose. */
	deVideo *GetRootVideo() const;
	
	/** \brief Video with filename or NULL if not loaded yet. */
	deVideo *GetVideoWith( const char *filename ) const;
	
	/** \brief Video with filename or NULL if not loaded yet. */
	deVideo *GetVideoWith( deVirtualFileSystem *vfs, const char *filename ) const;
	
	/** \brief Load video from file relative to base path. */
	deVideo *LoadVideo( const char *filename, const char *basePath, bool asynchron );
	
	/** \brief Load video from file relative to base path. */
	deVideo *LoadVideo( deVirtualFileSystem *vfs, const char *filename,
		const char *basePath, bool asynchron );
	
	/** \brief Save video to file. */
	void SaveVideo( deVideo *video, const char *filename );
	
	/** \brief Save video to file. */
	void SaveVideo( deVirtualFileSystem *vfs, deVideo *video, const char *filename );
	
	/**
	 * \brief Add loaded video.
	 * \warning This method is to be used only by the resource loader!
	 */
	void AddLoadedVideo( deVideo *video );
	
	
	
	/** \brief Create video decoder. */
	deVideoDecoder *CreateDecoder( deVideo *video );
	
	/** \brief Create audio decoder or NULL if video has no audio. */
	deVideoAudioDecoder *CreateAudioDecoder( deVideo *video );
	
	
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	virtual void SystemGraphicLoad();
	virtual void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	virtual void RemoveResource( deResource *resource );
	void RemoveDecoder( deVideoDecoder *decoder );
	void RemoveAudioDecoder( deVideoAudioDecoder *decoder );
	/*@}*/
};

#endif

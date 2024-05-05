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
class DE_DLL_EXPORT deVideoManager : public deFileResourceManager{
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

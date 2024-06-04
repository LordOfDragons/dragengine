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

#ifndef _DEOGLSHAREDVIDEOPLAYERLIST_H_
#define _DEOGLSHAREDVIDEOPLAYERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deGraphicOpenGl;
class deoglSharedVideoPlayer;
class deVideo;



/**
 * Shared video player list.
 * 
 * List of pointers to shared video players. The ordering of the shared video players
 * in the list is not defined and can change when shared video players are removed.
 * This allows to remove elements with constant time requirements.
 */
class deoglSharedVideoPlayerList{
private:
	deGraphicOpenGl &pOgl;
	decPointerList pList;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create shared video player list. */
	deoglSharedVideoPlayerList( deGraphicOpenGl &ogl );
	
	/** Clean up shared video player list. */
	~deoglSharedVideoPlayerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** Number of shared video players. */
	int GetCount() const;
	
	/**
	 * Shared video player with matching parameters.
	 * \details Creates the shared video player if not existing yet.
	 */
	deoglSharedVideoPlayer *GetVideoPlayerFor( deVideo *video, int currentFrame, float playbackSpeed );
	
	/**
	 * Update all shared video players.
	 * \details Called from the main thread.
	 */
	void UpdateAll( float elapsed );
	
	/**
	 * Update render thread counterparts of all shared video players.
	 * \details Called from the main thread during synchronizing.
	 */
	void SyncAllToRender();
	
	/**
	 * Remove shared video player.
	 * \details For use by deoglSharedVideoPlayer only.
	 */
	void RemoveSharedVideoPlayer( deoglSharedVideoPlayer *sharedVideoPlayer );
	/*@}*/
};

#endif

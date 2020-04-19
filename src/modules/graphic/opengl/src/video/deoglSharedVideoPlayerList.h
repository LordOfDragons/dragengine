/* 
 * Drag[en]gine OpenGL Graphic Module
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

#ifndef _DEOGLSHAREDVIDEOPLAYERLIST_H_
#define _DEOGLSHAREDVIDEOPLAYERLIST_H_

#include <dragengine/common/collection/decPointerList.h>

class deGraphicOpenGl;
class deoglSharedVideoPlayer;
class deVideo;



/**
 * \brief Shared video player list.
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
	/** \brief Create shared video player list. */
	deoglSharedVideoPlayerList( deGraphicOpenGl &ogl );
	
	/** \brief Clean up shared video player list. */
	~deoglSharedVideoPlayerList();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Number of shared video players. */
	int GetCount() const;
	
	/**
	 * \brief Shared video player with matching parameters.
	 * \details Creates the shared video player if not existing yet.
	 */
	deoglSharedVideoPlayer *GetVideoPlayerFor( deVideo *video, int currentFrame, float playbackSpeed );
	
	/**
	 * \brief Update all shared video players.
	 * \details Called from the main thread.
	 */
	void UpdateAll( float elapsed );
	
	/**
	 * \brief Update render thread counterparts of all shared video players.
	 * \details Called from the main thread during synchronizing.
	 */
	void SyncAllToRender();
	
	/**
	 * \brief Remove shared video player.
	 * \details For use by deoglSharedVideoPlayer only.
	 */
	void RemoveSharedVideoPlayer( deoglSharedVideoPlayer *sharedVideoPlayer );
	/*@}*/
};

#endif

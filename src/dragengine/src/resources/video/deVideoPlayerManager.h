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

#ifndef _DEVIDEOPLAYERMANAGER_H_
#define _DEVIDEOPLAYERMANAGER_H_

#include "../deResourceManager.h"
#include "../deResourceList.h"

class deEngine;
class deVideoPlayer;


/**
 * \brief Video Player Resource Manager.
 */
class deVideoPlayerManager : public deResourceManager{
private:
	deResourceList pVideoPlayers;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create new video player resource manager linked to the given engine. */
	deVideoPlayerManager( deEngine *engine );
	
	/** \brief Clean up video player resource manager and reports leaking resources. */
	~deVideoPlayerManager();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Count of video players. */
	int GetVideoPlayerCount() const;
	
	/** \brief Root video player resource for iteration purpose. */
	deVideoPlayer *GetRootVideoPlayer() const;
	
	/** \brief Create new video player. */
	deVideoPlayer *CreateVideoPlayer();
	
	/** \brief Release leaking resources and report them. */
	virtual void ReleaseLeakingResources();
	/*@}*/
	
	
	
	/** \name System Peer Management */
	/*@{*/
	void SystemAudioLoad();
	void SystemAudioUnload();
	void SystemGraphicLoad();
	void SystemGraphicUnload();
	/*@}*/
	
	
	
	/**
	 * \name Resource only Functions
	 * Those functions are only for resource objects and should never be
	 * called directly from an application.
	 */
	/*@{*/
	void RemoveResource( deResource *resource );
	/*@}*/
};

#endif

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

#ifndef _DEOGLSKINSTATECONTROLLER_H_
#define _DEOGLSKINSTATECONTROLLER_H_

#include <dragengine/common/collection/decObjectList.h>

class deoglRSkin;
class deoglSharedVideoPlayer;
class deoglSharedVideoPlayerList;
class deoglSkinState;
class deoglSkinTexture;
class deoglWorld;
class deVideoPlayer;



/**
 * \brief Skin state controller.
 */
class deoglSkinStateController{
private:
	deoglSharedVideoPlayer **pSharedVideoPlayers;
	int pSharedVideoPlayerCount;
	
	decObjectList pVideoPlayers;
	
	bool pHasCalculatedProperties;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create skin state controller. */
	deoglSkinStateController();
	
	/** \brief Clean up skin state controller. */
	~deoglSkinStateController();
	/*@}*/
	
	
	
	/** \name Management. */
	/*@{*/
	/** \brief Number of shared video players. */
	inline int GetVideoPlayerCount() const{ return pSharedVideoPlayerCount; }
	
	/** \brief Set number of video players. */
	void SetVideoPlayerCount( int count );
	
	/** \brief Video player by index or \em NULL if not existing. */
	deVideoPlayer *GetVideoPlayerAt( int index ) const;
	
	/** \brief Shared video player by index or \em NULL if not existing. */
	deoglSharedVideoPlayer *GetSharedVideoPlayerAt( int index ) const;
	
	/** \brief Set video player at index or \em NULL if not existing. */
	void SetVideoPlayerAt( int index, deVideoPlayer *videoPlayer );
	
	/** \brief Set shared video player at index or \em NULL if not existing. */
	void SetSharedVideoPlayerAt( int index, deoglSharedVideoPlayer *videoPlayer );
	
	
	
	/** \brief Has calculated properties. */
	inline bool GetHasCalculatedProperties() const{ return pHasCalculatedProperties; }
	
	/** \brief Set if has calculated properties. */
	void SetHasCalculatedProperties( bool hasCalculatedProperties );
	
	
	
	/** \brief Requires sync every frame update. */
	bool RequiresSyncEveryFrameUpdate() const;
	
	/** \brief Requires prepare renderables. */
	bool RequiresPrepareRenderables() const;
	
	/** \brief Init skin state. */
	void Init( deoglSkinState &skinState, deoglRSkin *skin, deoglWorld *world );
	
	/** \brief Init skin state. */
	void Init( deoglSkinState &skinState, deoglRSkin *skin, int textureIndex, deoglWorld *world );
	
	
	
	/** \brief Reset time. */
	void ResetTime();
	
	/** \brief Update time. */
	void AdvanceTime( float timeStep );
	
	/** \brief Synchronize to render. */
	void SyncToRender();
	
	/** \brief Clear. */
	void Clear();
	/*@}*/
	
	
	
private:
	void pUpdateSkinVideoPlayers( deoglSkinState &skinState, deoglSkinTexture &texture, deoglWorld &world );
};

#endif
